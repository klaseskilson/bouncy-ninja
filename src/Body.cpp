#include "Body.h"
#include <iostream>

// prepare static members
bool Body::mDebug = true;
GLShader* Body::basicShader;
std::vector<std::shared_ptr<Boundary>> Body::mBoundaries;

Body::Body()
{
    //loadObj("../assets/cube.obj");
    /*
    for(int x = 0; x < 3; x++)
    {
        for(int y = 0; y < 3; y++)
        {
            for(int z = 0; z < 3; z++)
            {
                mMasses.push_back(std::shared_ptr<Mass>(new Mass(glm::vec3((1.0-x), (1.0-y), (1.0-z)))));
            }
        }
    }*/
    
    //Rope
    for (int i = 0; i < 15; i++)
    {
        mMasses.push_back(std::shared_ptr<Mass>(new Mass(glm::vec3(0.1f*(float)i, 6.0f, 0.0f))));
        if (i > 0)
        {
            mMasses.at(i)->connectMass(mMasses.at(i - 1));
        }
    }
    mMasses.at(0)->setStatic(true);

}

Body::~Body()
{
    //mMasses.clear();
    //mBoundaries.clear();
}

void Body::draw()
{
    if (mDebug)
    {
        for (std::vector<std::shared_ptr<Mass>>::iterator it = mMasses.begin(); it != mMasses.end(); ++it)
        {
            (*it)->draw();
        }
    }
    else
    {
        // Update the transformation
        glm::mat4 mTransform = glm::mat4(1.0f);

        glUseProgram(Mass::getShader()->programID);

        // Send our transformation to the currently bound shader
        GLuint transformID = glGetUniformLocation(Mass::getShader()->programID, "M");
        glUniformMatrix4fv(transformID, 1, GL_FALSE, &mTransform[0][0]);

        glBindVertexArray(vao);
        // (mode, vertex count, type, element array buffer offset)
        glDrawElements(GL_TRIANGLES, indices.size() , GL_UNSIGNED_SHORT, (void*)0);
        glBindVertexArray(0);
    }
}

void Body::update(float timeDelta)
{
    for (std::vector<std::shared_ptr<Mass>>::iterator it = mMasses.begin(); it != mMasses.end(); ++it)
    {
        (*it)->update(timeDelta);
    }
}

void Body::addBoundary(std::shared_ptr<Boundary> b)
{
    mBoundaries.push_back(b);
}

std::vector<std::shared_ptr<Boundary>> Body::getBoundaries()
{
    return mBoundaries;
}

void Body::setShader(GLShader* shader)
{
    basicShader = shader;
}

GLShader* Body::getShader()
{
    return basicShader;
}

void Body::loadObj(const char * path)
{
    std::vector<glm::vec3> out_vertices;
    std::vector<glm::vec2> out_uvs;
    std::vector<glm::vec3> out_normals;

    loadOBJ(path, out_vertices, out_uvs, out_normals);

    indexVBO(out_vertices, out_uvs, out_normals, indices, vertices, uvs, normals);


    for (int i = 0; i < vertices.size(); i++)
    {
        mMasses.push_back(std::shared_ptr<Mass>(new Mass(vertices.at(i))));
        vertexarray.push_back(vertices.at(i).x);
        vertexarray.push_back(vertices.at(i).y);
        vertexarray.push_back(vertices.at(i).z);
        vertexarray.push_back(normals.at(i).x);
        vertexarray.push_back(normals.at(i).y);
        vertexarray.push_back(normals.at(i).z);
        vertexarray.push_back(uvs.at(i).x);
        vertexarray.push_back(uvs.at(i).y);
    }

    //Connect masses with other masses using the index list
    for (int i = 0; i < indices.size()/3; i++)
    {
        std::cout << "Connecting mass: " << indices.at(3 * i) << ", " << indices.at(3 * i + 1) << ", " << indices.at(3 * i + 2) << ".\n";
        //indices.at(3*i) ska connecta med indices.at(3*i+1)
        mMasses.at(indices.at(3 * i))->connectMass(mMasses.at(indices.at(3 * i + 1)));

        //indices.at(3*i+1) ska connecta med indices.at(3*i+2)
        mMasses.at(indices.at(3 * i + 1))->connectMass(mMasses.at(indices.at(3 * i + 2)));

        //indices.at(3*i+2) ska connecta med indices.at(3*i)
        mMasses.at(indices.at(3 * i + 2))->connectMass(mMasses.at(indices.at(3 * i)));

    }
    mMasses.at(0)->setStatic(true);


    numberOfTriangles = indices.size() / 3;
    numberOfVertices = vertices.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &indexbuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    glBufferData(GL_ARRAY_BUFFER, 8 * vertices.size() * sizeof(GLfloat), &(vertexarray[0]), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(GLfloat), (void*)0); // xyz coordinates
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // normals
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
        8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // texcoords

    // Activate the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
    // Present our vertex indices to OpenGL
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned short), &(indices[0]), GL_STATIC_DRAW);

    // Deactivate (unbind) the VAO and the buffers again.
    // Do NOT unbind the buffers while the VAO is still bound.
    // The index buffer is an essential part of the VAO state.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
