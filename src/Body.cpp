#include "Body.h"
#include <iostream>
bool Body::mDebug = true;
GLShader* Body::basicShader;

Body::Body()
{
    // loadObj("../assets/suzanne.obj");

    for(int x = 0; x < 3; x++)
    {
        for(int y = 0; y < 3; y++)
        {
            for(int z = 0; z < 3; z++)
            {
                mMasses.push_back(std::shared_ptr<Mass>(new Mass(glm::vec3((1.0-x), (1.0-y), (1.0-z)))));
            }
        }
    }

    // Mass* tmpMass = new Mass(glm::vec3(-1.0f,1.0f,1.0f));
    // Mass* tmpMass2 = new Mass(glm::vec3(1.0f,1.0f,1.0f));
    // Mass* tmpMass3 = new Mass(glm::vec3(1.0f, 1.0f, -1.0f));
    // Mass* tmpMass4 = new Mass(glm::vec3(-1.5f, 1.0f, -1.0f));
    // Mass* tmpMass5 = new Mass(glm::vec3(-1.0f, -1.0f, 1.0f));
    // Mass* tmpMass6 = new Mass(glm::vec3(1.0f, -1.0f, 1.0f));
    // Mass* tmpMass7 = new Mass(glm::vec3(1.0f, -1.0f, -1.0f));
    // Mass* tmpMass8 = new Mass(glm::vec3(-1.5f, -1.0f, -1.0f));

    //tmpMass4->setVelocity(glm::vec3(-8.0f,0.0f,0.0f));

    //tmpMass->setStatic(true);

    //mMasses.at(1)->connectMass(mMasses.at(2));
    /*tmpMass->connectMass(tmpMass3);
    tmpMass->connectMass(tmpMass4);
    tmpMass->connectMass(tmpMass5);
    tmpMass->connectMass(tmpMass6);
    tmpMass->connectMass(tmpMass7);
    tmpMass->connectMass(tmpMass8);

    tmpMass2->connectMass(tmpMass3);
    tmpMass2->connectMass(tmpMass4);
    tmpMass2->connectMass(tmpMass5);
    tmpMass2->connectMass(tmpMass6);
    tmpMass2->connectMass(tmpMass7);
    tmpMass2->connectMass(tmpMass8);

    tmpMass3->connectMass(tmpMass4);
    tmpMass3->connectMass(tmpMass5);
    tmpMass3->connectMass(tmpMass6);
    tmpMass3->connectMass(tmpMass7);
    tmpMass3->connectMass(tmpMass8);

    tmpMass4->connectMass(tmpMass5);
    tmpMass4->connectMass(tmpMass6);
    tmpMass4->connectMass(tmpMass7);
    tmpMass4->connectMass(tmpMass8);

    tmpMass5->connectMass(tmpMass6);
    tmpMass5->connectMass(tmpMass7);
    tmpMass5->connectMass(tmpMass8);

    tmpMass6->connectMass(tmpMass7);
    tmpMass6->connectMass(tmpMass8);

    tmpMass7->connectMass(tmpMass8);

    mMasses.push_back(std::shared_ptr<Mass>(tmpMass));
    mMasses.push_back(std::shared_ptr<Mass>(tmpMass2));
    mMasses.push_back(std::shared_ptr<Mass>(tmpMass3));
    mMasses.push_back(std::shared_ptr<Mass>(tmpMass4));
    mMasses.push_back(std::shared_ptr<Mass>(tmpMass5));
    mMasses.push_back(std::shared_ptr<Mass>(tmpMass6));
    mMasses.push_back(std::shared_ptr<Mass>(tmpMass7));
    mMasses.push_back(std::shared_ptr<Mass>(tmpMass8));*/
}

Body::~Body()
{
    mMasses.clear();
    mBoundaries.clear();
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
        vertexarray.push_back(vertices.at(i).x);
        vertexarray.push_back(vertices.at(i).y);
        vertexarray.push_back(vertices.at(i).z);
        vertexarray.push_back(normals.at(i).x);
        vertexarray.push_back(normals.at(i).y);
        vertexarray.push_back(normals.at(i).z);
        vertexarray.push_back(uvs.at(i).x);
        vertexarray.push_back(uvs.at(i).y);
    }

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
