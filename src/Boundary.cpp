#include "Boundary.h"

Boundary::Boundary(glm::vec3 a, glm::vec3 b)
{
    // set corners
    mA = a;
    mB = b;

    // setup our box showing the boundary's hitbox
    glm::vec3 diff = mB - mA;
    float xsize = diff.x;
    float ysize = diff.y;
    float zsize = diff.z;
    // create vertices
    GLfloat vertex_array_data[] = {
        // front
        mB.x, mA.y, mB.z, 0, 0, 1, 1, 0, // 0
        mB.x, mB.y, mB.z, 0, 0, 1, 1, 1, // 1
        mA.x, mB.y, mB.z, 0, 0, 1, 0, 1, // 2
        mA.x, mA.y, mB.z, 0, 0, 1, 0, 0, // 3
        // back
        mB.x, mA.y, mA.z, 0, 0, -1, 0, 0, // 4
        mB.x, mB.y, mA.z, 0, 0, -1, 0, 1, // 5
        mA.x, mB.y, mA.z, 0, 0, -1, 1, 1, // 6
        mA.x, mA.y, mA.z, 0, 0, -1, 1, 0, // 7
        // top
        mB.x, mB.y, mB.z, 0, 1, 0, 1, 0, // 8
        mB.x, mB.y, mA.z, 0, 1, 0, 1, 1, // 9
        mA.x, mB.y, mA.z, 0, 1, 0, 0, 1, // 10
        mA.x, mB.y, mB.z, 0, 1, 0, 0, 0, // 11
        // bottom
        mB.x, mA.y, mB.z, 0, -1, 0, 0, 0, // 12
        mB.x, mA.y, mA.z, 0, -1, 0, 1, 0, // 13
        mA.x, mA.y, mA.z, 0, -1, 0, 1, 1, // 14
        mA.x, mA.y, mB.z, 0, -1, 0, 0, 1, // 15
        // left
        mA.x, mA.y, mB.z, -1, 0, 0, 1, 0, // 16
        mA.x, mB.y, mB.z, -1, 0, 0, 1, 1, // 17
        mA.x, mB.y, mA.z, -1, 0, 0, 0, 1, // 18
        mA.x, mA.y, mA.z, -1, 0, 0, 0, 0, // 19
        // right
        mB.x, mA.y, mB.z, 1, 0, 0, 0, 0, // 20
        mB.x, mB.y, mB.z, 1, 0, 0, 0, 1, // 21
        mB.x, mB.y, mA.z, 1, 0, 0, 1, 1, // 22
        mB.x, mA.y, mA.z, 1, 0, 0, 1, 0, // 23
    };

    GLuint index_array_data[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // back
        6, 5, 4,
        4, 7, 6,
        // top
        8, 9, 10,
        10, 11, 8,
        // bottom
        14, 13, 12,
        12, 15, 14,
        // left
        16, 17, 18,
        18, 19, 16,
        // right
        20, 23, 22,
        22, 21, 20
    };

    mGlVertexArray = vertex_array_data;
    mGlIndexArray = index_array_data;

    mGlNumberOfVertices = 24;
    mGlNumberOfTriangles = 12;

    glGenVertexArrays(1, &mGlVao);
    glBindVertexArray(mGlVao);

    glGenBuffers(1, &mGlVertexBuffer);
    glGenBuffers(1, &mGlIndexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, mGlVertexBuffer);

    glBufferData(GL_ARRAY_BUFFER,8 * mGlNumberOfVertices * sizeof(GLfloat),
        mGlVertexArray, GL_STATIC_DRAW);

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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGlIndexBuffer);
    // Present our vertex indices to OpenGL
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mGlNumberOfTriangles * sizeof(GLuint),
        mGlIndexArray, GL_STATIC_DRAW);

    // Deactivate (unbind) the VAO and the buffers again.
    // Do NOT unbind the buffers while the VAO is still bound.
    // The index buffer is an essential part of the VAO state.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Boundary::~Boundary()
{
}

void Boundary::setShader(GLShader* s)
{
    mShader = s;
}

float Boundary::getTop()
{
    return mB.y;
}

float Boundary::getBottom()
{
    return mA.y;
}

void Boundary::getProperPosition(glm::vec3 &pos, glm::vec3 oldPos, glm::vec3 &vel)
{
    // detect if pos is within the boundary's limits
    if ((pos.x > mA.x && pos.x < mB.x) &&
        (pos.y > mA.y && pos.y < mB.y) &&
        (pos.z > mA.z && pos.z < mB.z))
    {
        // Here, we are making sure that the speed of our object isn't
        // increasing in the direction of our boundary.
        // It's about to get ugly. First we see what direction the body is going
        // for (which is what we do in this mess of if statements), and then we
        // modify the velocity based on that knowledge.

        // positive x-side
        if (pos.y > mA.y && pos.y < mB.y && pos.z > mA.z && pos.z < mB.z && oldPos.x > mB.x)
        {
            // limit speed
            vel.x = fmax(0.0f, vel.x);
            // "friction"
            // vel.y = mFriction * vel.y;
            // vel.z = mFriction * vel.z;
            pos.x = oldPos.x;
        }
        // negative x-side
        if (pos.y > mA.y && pos.y < mB.y && pos.z > mA.z && pos.z < mB.z && oldPos.x < mA.x)
        {
            // limit speed
            vel.x = fmin(0.0f, vel.x);
            // "friction"
            vel.y = mFriction * vel.y;
            vel.z = mFriction * vel.z;
            pos.x = oldPos.x;
        }
        // positive y-side
        if (pos.x > mA.x && pos.x < mB.x && pos.z > mA.z && pos.z < mB.z && oldPos.y > mB.y)
        {
            // limit speed
            vel.y = fmax(0.0f, vel.y);
            // "friction"
            vel.x = mFriction * vel.x;
            vel.z = mFriction * vel.z;
            pos.y = oldPos.y;
        }
        // negative y-side
        if (pos.x > mA.x && pos.x < mB.x && pos.z > mA.z && pos.z < mB.z && oldPos.y < mA.y)
        {
            // limit speed
            vel.y = fmin(0.0f, vel.y);
            // "friction"
            vel.x = mFriction * vel.x;
            vel.z = mFriction * vel.z;
            pos.y = oldPos.y;
        }
        // positive z-side
        if (pos.x > mA.x && pos.x < mB.x && pos.y > mA.y && pos.y < mB.y && oldPos.z > mB.z)
        {
            // limit speed
            vel.z = fmax(0.0f, vel.z);
            // "friction"
            vel.x = mFriction * vel.x;
            vel.y = mFriction * vel.y;
            pos.z = oldPos.z;
        }
        // negative z-side
        if (pos.x > mA.x && pos.x < mB.x && pos.y > mA.y && pos.y < mB.y && oldPos.z < mA.z)
        {
            // limit speed
            vel.z = fmin(0.0f, vel.z);
            // "friction"
            vel.x = mFriction * vel.x;
            vel.y = mFriction * vel.y;
            pos.z = oldPos.z;
        }

        // Aaaaand adjust the position
        pos.y = oldPos.y;
    }
}

void Boundary::draw()
{
    glUseProgram(mShader->programID);

    // Use identity matrix as our transform matrix
    glm::mat4 transf = glm::mat4();
    // Send our transformation to the currently bound shader
    GLuint transformID = glGetUniformLocation(mShader->programID, "M");
    glUniformMatrix4fv(transformID, 1, GL_FALSE, &transf[0][0]);

    glBindVertexArray(mGlVao);
    // (mode, vertex count, type, element array buffer offset)
    glDrawElements(GL_TRIANGLES, 3 * mGlNumberOfTriangles, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}
