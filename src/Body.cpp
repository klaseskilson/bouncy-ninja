#include "Body.h"
#include <iostream>

// prepare static members
bool Body::mDebug = true;
GLShader* Body::basicShader;

Body::Body()
{
    // loadObj("../assets/suzanne.obj");
    createCube(4, 0.8, glm::vec3(1.0,1.0,1.0));

    // mMasses.at(0)->setStatic(true);
    // mMasses.at(2)->setStatic(true);
    mMasses.at(0)->setVelocity(glm::vec3(-10.0f, 0.0f, 0.0f));
    mMasses.at(1)->setVelocity(glm::vec3(-10.0f, 0.0f, 0.0f));
    mMasses.at(2)->setVelocity(glm::vec3(-10.0f, 0.0f, 0.0f));
    mMasses.at(3)->setVelocity(glm::vec3(-10.0f, 0.0f, 0.0f));
    mMasses.at(4)->setVelocity(glm::vec3(-10.0f, 0.0f, 0.0f));
    mMasses.at(25+0)->setVelocity(glm::vec3(-10.0f, 0.0f, 0.0f));
    mMasses.at(25+1)->setVelocity(glm::vec3(-10.0f, 0.0f, 0.0f));
    mMasses.at(25+2)->setVelocity(glm::vec3(-10.0f, 0.0f, 0.0f));
    mMasses.at(25+3)->setVelocity(glm::vec3(-10.0f, 0.0f, 0.0f));
    mMasses.at(25+4)->setVelocity(glm::vec3(-10.0f, 0.0f, 0.0f));

    mMasses.at(0)->connectMass(mMasses.at(4));
    mMasses.at(0)->connectMass(mMasses.at(20));
    mMasses.at(0)->connectMass(mMasses.at(100));
    mMasses.at(4)->connectMass(mMasses.at(24));

    mMasses.at(4)->connectMass(mMasses.at(104));
    mMasses.at(20)->connectMass(mMasses.at(24));
    mMasses.at(20)->connectMass(mMasses.at(120));
    mMasses.at(24)->connectMass(mMasses.at(124));

    mMasses.at(100)->connectMass(mMasses.at(104));
    mMasses.at(100)->connectMass(mMasses.at(120));
    mMasses.at(120)->connectMass(mMasses.at(124));
    mMasses.at(104)->connectMass(mMasses.at(124));

    //Rope
    // for (int i = 0; i < 15; i++)
    // {
    //     mMasses.push_back(std::shared_ptr<Mass>(new Mass(glm::vec3(0.1f*(float)i, 6.0f, 0.0f))));
    //     if (i > 0)
    //     {
    //         mMasses.at(i)->connectMass(mMasses.at(i - 1));
    //     }
    // }
    // mMasses.at(0)->setStatic(true);
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
        updateGL();
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
        (*it)->update(timeDelta, mBoundaries);
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

void Body::createCube(int nMasses, float massDistance, glm::vec3 sPoint)
{
    for(int x = 0; x < nMasses; x++)
    {
        for(int y = 0; y < nMasses; y++)
        {
            for(int z = 0; z < nMasses; z++)
            {
                mMasses.push_back(std::shared_ptr<Mass>(
                    new Mass(glm::vec3(sPoint.x-x*massDistance, sPoint.y-y*massDistance, sPoint.z-z*massDistance))));
            }
        }
    }

    float radius = glm::length(mMasses.at(0)->getPosition() - mMasses.at(nMasses * (nMasses + 1) + 1)->getPosition());

    for(std::vector<std::shared_ptr<Mass>>::iterator it = mMasses.begin(); it != mMasses.end(); ++it)
    {
        for(std::vector<std::shared_ptr<Mass>>::iterator it2 = mMasses.begin(); it2 != mMasses.end(); ++it2)
        {
            if((glm::length((*it)->getPosition() - (*it2)->getPosition())) <= radius &&
                (glm::length((*it)->getPosition() - (*it2)->getPosition())) != 0.0)
            {
               (*it)->connectMass(*it2);
            }
        }
        //std::cout << *it << " " << *std::next(it,1) << '\n';
    }
    
    //OpenGL Initialization
    boxSize = nMasses;
    
    updateVertices();

    numberOfTriangles = indices.size() / 3;
    numberOfVertices = boxSize*boxSize*6;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &indexbuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    glBufferData(GL_ARRAY_BUFFER, 8 * numberOfVertices * sizeof(GLfloat), &(vertexarray[0]), GL_STATIC_DRAW);

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

void Body::updateVertices()
{
    vertexarray.clear();

    //OpenGL Initialization
    //Front
    for (int i = 0; i < boxSize*boxSize; i++)
    {
        vertices.push_back(mMasses.at(i)->getPosition());

        vertexarray.push_back(mMasses.at(i)->getPosition().x);
        vertexarray.push_back(mMasses.at(i)->getPosition().y);
        vertexarray.push_back(mMasses.at(i)->getPosition().z);

        vertexarray.push_back(1.0f);
        vertexarray.push_back(0.0f);
        vertexarray.push_back(0.0f);

        vertexarray.push_back(1.0f);
        vertexarray.push_back(0.0f);
    }

    //Back
    for (int i = (boxSize*boxSize)*(boxSize - 1); i < (boxSize*boxSize*boxSize); i++)
    {
        vertices.push_back(mMasses.at(i)->getPosition());

        vertexarray.push_back(mMasses.at(i)->getPosition().x);
        vertexarray.push_back(mMasses.at(i)->getPosition().y);
        vertexarray.push_back(mMasses.at(i)->getPosition().z);

        vertexarray.push_back(-1.0f);
        vertexarray.push_back(0.0f);
        vertexarray.push_back(0.0f);

        vertexarray.push_back(1.0f);
        vertexarray.push_back(0.0f);
    }
    //Right
    for (int i = boxSize - 1; i < boxSize*boxSize*boxSize; i += boxSize)
    {
        vertices.push_back(mMasses.at(i)->getPosition());

        vertexarray.push_back(mMasses.at(i)->getPosition().x);
        vertexarray.push_back(mMasses.at(i)->getPosition().y);
        vertexarray.push_back(mMasses.at(i)->getPosition().z);

        vertexarray.push_back(0.0f);
        vertexarray.push_back(0.0f);
        vertexarray.push_back(-1.0f);

        vertexarray.push_back(1.0f);
        vertexarray.push_back(0.0f);
    }

    //left
    for (int i = 0; i < (boxSize*boxSize*boxSize) - boxSize + 1; i += boxSize)
    {
        vertices.push_back(mMasses.at(i)->getPosition());

        vertexarray.push_back(mMasses.at(i)->getPosition().x);
        vertexarray.push_back(mMasses.at(i)->getPosition().y);
        vertexarray.push_back(mMasses.at(i)->getPosition().z);

        vertexarray.push_back(0.0f);
        vertexarray.push_back(0.0f);
        vertexarray.push_back(1.0f);

        vertexarray.push_back(1.0f);
        vertexarray.push_back(0.0f);
    }

    //top
    for (int i = 0; i < boxSize; i++)
    {
        for (int j = 0; j < boxSize*boxSize*boxSize; j += boxSize*boxSize)
        {
            vertices.push_back(mMasses.at(i + j)->getPosition());

            vertexarray.push_back(mMasses.at(i + j)->getPosition().x);
            vertexarray.push_back(mMasses.at(i + j)->getPosition().y);
            vertexarray.push_back(mMasses.at(i + j)->getPosition().z);

            vertexarray.push_back(0.0f);
            vertexarray.push_back(1.0f);
            vertexarray.push_back(0.0f);

            vertexarray.push_back(1.0f);
            vertexarray.push_back(0.0f);
        }

    }
    //bottom
    for (int i = boxSize * 2; i < boxSize * 3; i++)
    {
        for (int j = 0; j < boxSize*boxSize*boxSize; j += boxSize*boxSize)
        {
            vertices.push_back(mMasses.at(i + j)->getPosition());

            vertexarray.push_back(mMasses.at(i + j)->getPosition().x);
            vertexarray.push_back(mMasses.at(i + j)->getPosition().y);
            vertexarray.push_back(mMasses.at(i + j)->getPosition().z);

            vertexarray.push_back(0.0f);
            vertexarray.push_back(-1.0f);
            vertexarray.push_back(0.0f);

            vertexarray.push_back(1.0f);
            vertexarray.push_back(0.0f);
        }

    }

    for (int k = 0; k < 6; k++)
    {
        for (int i = 0; i < boxSize - 1; i++)
        {
            for (int j = 0; j < boxSize - 1; j++)
            {
                unsigned short n1 = i + boxSize * j + boxSize*boxSize*k;
                unsigned short n2 = i + boxSize * (j + 1) + boxSize*boxSize * k;
                unsigned short n3 = (i + 1 + boxSize * (j + 1)) + boxSize* boxSize* k;
                unsigned short n4 = i + 1 + boxSize * j + boxSize*boxSize * k;

                //TODO: Calculate normals
                /*
                glm::vec3 normal1 = glm::normalize(glm::cross(vertices.at(n2) - vertices.at(n1), vertices.at(n4) - vertices.at(n1)));
                glm::vec3 normal2 = glm::normalize(glm::cross(vertices.at(n1) - vertices.at(n2), vertices.at(n3) - vertices.at(n2)));
                glm::vec3 normal3 = glm::normalize(glm::cross(vertices.at(n2) - vertices.at(n3), vertices.at(n4) - vertices.at(n3)));
                glm::vec3 normal4 = glm::normalize(glm::cross(vertices.at(n3) - vertices.at(n4), vertices.at(n1) - vertices.at(n4)));
                
                vertexarray.at(8 * n1 + 3) = normal1.x;
                vertexarray.at(8 * n1 + 4) = normal1.y;
                vertexarray.at(8 * n1 + 5) = normal1.z;
                
                vertexarray.at(8 * n2 + 3) = normal2.x;
                vertexarray.at(8 * n2 + 4) = normal2.y;
                vertexarray.at(8 * n2 + 5) = normal2.z;

                vertexarray.at(8 * n3 + 3) = normal3.x;
                vertexarray.at(8 * n3 + 4) = normal3.y;
                vertexarray.at(8 * n3 + 5) = normal3.z;

                vertexarray.at(8 * n4 + 3) = normal4.x;
                vertexarray.at(8 * n4 + 4) = normal4.y;
                vertexarray.at(8 * n4 + 5) = normal4.z;
                */
                indices.push_back(n1);
                indices.push_back(n2);
                indices.push_back(n3);
                indices.push_back(n3);
                indices.push_back(n4);
                indices.push_back(n1);
            }
        }
    }

}

void Body::updateGL()
{
    updateVertices();
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, 8 * numberOfVertices * sizeof(GLfloat), &(vertexarray[0]), GL_STATIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}

void Body::loadObj(const char * path)
{
    std::vector<glm::vec3> out_vertices;
    std::vector<glm::vec2> out_uvs;
    std::vector<glm::vec3> out_normals;

    loadOBJ(path, out_vertices, out_uvs, out_normals);

    //indexVBO(out_vertices, out_uvs, out_normals, indices, vertices, uvs, normals);


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
        std::cout << "Connecting mass: " << indices.at(3 * i) << ", "
            << indices.at(3 * i + 1) << ", " << indices.at(3 * i + 2) << ".\n";

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
