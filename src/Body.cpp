#include "Body.h"
#include <iostream>

// prepare static members
bool Body::mDebug = true;
GLShader* Body::basicShader;

Body::Body()
{
    // loadObj("../assets/suzanne.obj");
    createBox(3 ,6,5, 0.3, glm::vec3(1.0,1.0,1.0));

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
}

Body::~Body()
{
    mMasses.clear();
    mBoundaries.clear();
}

void Body::hit()
{
    for (std::vector<std::shared_ptr<Mass>>::iterator it = mMasses.begin(); it != mMasses.end(); ++it)
    {
        (*it)->setVelocity(glm::vec3(1.0f));
    }
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

// TODO: implement width and make the rope 2*2*length
void Body::createRope(int length, float width, glm::vec3 sPoint)
{
    for (int i = 0; i < length; i++)
    {
        mMasses.push_back(std::shared_ptr<Mass>(new Mass(glm::vec3(0.1f*(float)i, 6.0f, 0.0f))));
        if (i > 0)
        {
            mMasses.at(i)->connectMass(mMasses.at(i - 1));
        }
    }
    mMasses.at(0)->setStatic(true);
}

void Body::createBox(int x, int y, int z, float massDistance, glm::vec3 sPoint)
{
    xSize = x;
    ySize = y;
    zSize = z;
    for (int z = 0; z < zSize; z++)
    {
        for (int y = 0; y < ySize; y++)
        {
            for (int x = 0; x < xSize; x++)
            {
                mMasses.push_back(std::shared_ptr<Mass>(
                    new Mass(glm::vec3(sPoint.x+x*massDistance, sPoint.y-y*massDistance, sPoint.z+z*massDistance))));
            }
        }
    }

    float radius = glm::length(mMasses.at(0)->getPosition() - mMasses.at(zSize * (ySize + 1) + 1)->getPosition());

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
    // connect the corners, only for nMasses > 2
    if (xSize > 2 && ySize > 2 && zSize > 2)
    {
        // we are trying to connect the corners, which are found like this
        //         ySize*xSize*(zSize-1) +------+ xSize*ySize*(zSize-1)+xSize-1
        //                             .'|    .'|
        //                        0  +---+--+'  | xSize-1
        // xSize*((ySize*zSize)-1)   |   +--+---+ (xSize*ySize*zSize)-1
        //                           | .'   | .'
        //          (ySize-1)*xSize  +------+' (ySize*xSize)-1

        int n = xSize;
        
        // this could be done in a neat loop
        mMasses.at(0)->connectMass(mMasses.at(xSize-1));
        mMasses.at(0)->connectMass(mMasses.at((ySize-1)*xSize));
        mMasses.at(0)->connectMass(mMasses.at(ySize*xSize*(zSize-1)));
        mMasses.at(xSize - 1)->connectMass(mMasses.at((ySize*xSize) - 1));
        mMasses.at(xSize - 1)->connectMass(mMasses.at(xSize*ySize*(zSize - 1) + xSize - 1));
        mMasses.at((ySize*xSize) - 1)->connectMass(mMasses.at((ySize - 1)*xSize));
        mMasses.at((ySize*xSize) - 1)->connectMass(mMasses.at((xSize*ySize*zSize) - 1));
        mMasses.at((ySize - 1)*xSize)->connectMass(mMasses.at(xSize*((ySize*zSize) - 1)));
        mMasses.at(ySize*xSize*(zSize - 1))->connectMass(mMasses.at(xSize*ySize*(zSize - 1) + xSize - 1));
        mMasses.at(ySize*xSize*(zSize - 1))->connectMass(mMasses.at(xSize*((ySize*zSize) - 1)));
        mMasses.at(xSize*ySize*(zSize - 1) + xSize - 1)->connectMass(mMasses.at((xSize*ySize*zSize) - 1));
        mMasses.at((xSize*ySize*zSize) - 1)->connectMass(mMasses.at(xSize*((ySize*zSize) - 1)));
        
    }



    //OpenGL Initialization
    updateVertices();

    numberOfTriangles = indices.size() / 3;
    numberOfVertices = xSize*ySize*2 + ySize*zSize*2 + zSize*xSize*2;

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
    vertices.clear();
    indices.clear();

    //OpenGL Initialization
    //Front
    for (int i = 0; i < xSize*ySize; i++)
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

    //Back
    for (int i = (xSize*ySize)*(zSize - 1); i < (xSize * ySize * zSize); i++)
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
    //Right
    for (int i = xSize - 1; i < xSize*ySize*zSize; i += xSize)
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

    //left
    for (int i = 0; i < (xSize*ySize*zSize) - xSize + 1; i += xSize)
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

    //top
    for (int i = 0; i < xSize; i++)
    {
        for (int j = 0; j < xSize*ySize*zSize; j += xSize*ySize)
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
    for (int i = xSize * 2; i < xSize * 3; i++)
    {
        for (int j = 0; j < xSize*ySize*zSize; j += xSize*ySize)
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

    for (int k = 0; k < 2; k++)
    {
        for (int i = 0; i < xSize - 1; i++)
        {
            for (int j = 0; j < ySize - 1; j++)
            {
                unsigned short n1 = i + j*xSize               + xSize*ySize*k;
                unsigned short n2 = i + xSize * (j + 1)       + xSize*ySize * k;
                unsigned short n3 = (i + 1 + xSize * (j + 1)) + xSize*ySize* k;
                unsigned short n4 = i + 1 + xSize * j         + xSize*ySize * k;

                //TODO: Calculate normals
                indices.push_back(n1);
                indices.push_back(n2);
                indices.push_back(n3);
                indices.push_back(n3);
                indices.push_back(n4);
                indices.push_back(n1);
            }
        }
    }
    for (int k = 0; k < 2; k++)
    {
        for (int i = 0; i < zSize - 1; i++)
        {
            for (int j = 0; j < ySize - 1; j++)
            {
                unsigned short n1 = i + j*zSize + 2 * xSize*ySize + ySize*zSize*k;
                unsigned short n2 = i + zSize * (j + 1) + xSize*ySize * 2 + ySize*zSize*k;
                unsigned short n3 = (i + 1 + zSize * (j + 1)) + xSize*ySize * 2 + ySize*zSize*k;
                unsigned short n4 = i + 1 + zSize * j + xSize*ySize * 2 + ySize*zSize*k;

                //TODO: Calculate normals

                indices.push_back(n1);
                indices.push_back(n2);
                indices.push_back(n3);
                indices.push_back(n3);
                indices.push_back(n4);
                indices.push_back(n1);
            }
        }
    }

    //Top and bottom
    for (int k = 0; k < 2; k++)
    {
        for (int i = 0; i < xSize - 1; i++)
        {
            for (int j = 0; j < zSize - 1; j++)
            {
                unsigned short n1 = i + j*xSize               + xSize*ySize * 2 + ySize*zSize * 2 + xSize*zSize*k;
                unsigned short n2 = i + xSize * (j + 1)       + xSize*ySize * 2 + ySize*zSize * 2 + xSize*zSize*k;
                unsigned short n3 = (i + 1 + xSize * (j + 1)) + xSize*ySize * 2 + ySize*zSize * 2 + xSize*zSize*k;
                unsigned short n4 = i + 1 + xSize * j         + xSize*ySize * 2 + ySize*zSize * 2 + xSize*zSize*k;

                //TODO: Calculate normals

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
