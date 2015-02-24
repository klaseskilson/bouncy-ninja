#include "Mass.h"
GLShader*  Mass::basicShader;
bool Mass::gravityActive;

Mass::Mass()
{
    mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    mInitialPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    createDebugBox(0.2f, 0.2f, 0.2f);
}

Mass::Mass(glm::vec3 pos, float spring, float damping)
{
    mDampingConstant = damping;
    mSpringConstant = spring;
    mPosition = pos;
    mInitialPosition = pos;
    createDebugBox(0.2f, 0.2f, 0.2f);
    mVelocity = glm::vec3(0.0f);
}

Mass::~Mass()
{

    //mConnectedMasses.clear();
}

void Mass::update(float timeDelta, std::vector<std::shared_ptr<Boundary>> boundaries)
{
    if (!mIsStatic)
    {
        // collision detection needs this
        glm::vec3 oldPos = mPosition;

        // numerical integration method
        explicitEuler(timeDelta);

        // collision detection!
        for (std::vector<std::shared_ptr<Boundary>>::iterator b = boundaries.begin(); b != boundaries.end(); ++b)
        {
            (*b)->getProperPosition(mPosition, oldPos, mVelocity);
        }
    }
}

glm::vec3 Mass::accel(glm::vec3 prevVelocity, glm::vec3 prevPosition)
{
    glm::vec3 F = glm::vec3(0.0f);

    if (gravityActive)
    {
        F = F + glm::vec3(0.0f, -9.81f, 0.0f) * mMass;
    }

    // For each connected mass, calculate the force
    for (std::vector<Mass*>::iterator it = mConnectedMasses.begin(); it != mConnectedMasses.end(); ++it)
    {
        // vector from this point to the (*it) point
        glm::vec3 toPoint = prevPosition - (*it)->getPosition();

        // get the length of the spring in rest
        float springLength = glm::length((*it)->getInitialPosition() - mInitialPosition);

        // the spring's initial position as a vector
        glm::vec3 springVector = glm::normalize(toPoint) * springLength;

        //Spring force, Hook's
        F = F - (toPoint - springVector) * mSpringConstant;

        //Difference in velocity
        glm::vec3 velocityDifference = ((*it)->getVelocity() - prevVelocity);

        //Damping
        F = F + (velocityDifference) * mDampingConstant;
    }

    // newton III: F = m*a
    return F/mMass;
}

void Mass::explicitEuler(float h)
{
    glm::vec3 a = accel(mVelocity, mPosition);
    mVelocity = mVelocity + a * h;
    mPosition = mPosition + mVelocity * h;
}

void Mass::implicitEuler(glm::vec3 force, float h)
{
    glm::vec3 H = force + force * h;
    glm::vec3 nextVelocity = mVelocity + mVelocity * h;
    glm::vec3 deltaVelocity = nextVelocity - mVelocity;
    glm::vec3 nextForce = force + H * (mVelocity + deltaVelocity) * h;
    glm::vec3 a = nextForce / mMass;
    mVelocity = mVelocity + a * h;
    //std::cout << mVelocity << "\n";
    mPosition = mPosition + mVelocity * h;
}


void Mass::rungeKutta(float h)
{
    glm::vec3 kv1 = accel(mVelocity, mPosition);
    glm::vec3 kx1 = mVelocity;

    glm::vec3 kv2 = accel(mVelocity + kv1 * (h / 2.0f), mPosition + kx1 * (h / 2.0f));
    glm::vec3 kx2 = mVelocity + (h / 2.0f) * kv1;

    glm::vec3 kv3 = accel(mVelocity + kv2 * (h / 2.0f), mPosition + kx2 * (h / 2.0f));
    glm::vec3 kx3 = mVelocity + (h / 2.0f) * kv2;

    glm::vec3 kv4 = accel(mVelocity + kv3 * h, mPosition + kx3 * h);
    glm::vec3 kx4 = mVelocity + h * kv3;

    mVelocity = mVelocity + (h / 6.0f) * (kv1 + (2.0f * kv2) + (2.0f * kv3) + kv4);
    mPosition = mPosition + (h / 6.0f) * (kx1 + (2.0f * kx2) + (2.0f * kx3) + kx4);
}

void Mass::connectMass(std::shared_ptr<Mass> m)
{
    bool exists = false;

    for(std::vector<Mass*>::iterator it = mConnectedMasses.begin(); it != mConnectedMasses.end(); ++it)
    {
        if(m->getPosition() == (*it)->getPosition())
        {
            exists = true;
        }
    }

    if(!exists)
    {
        mConnectedMasses.push_back(m.get());
        m->mConnectedMasses.push_back(this);
    }
}

void Mass::setStatic(bool b)
{
    mIsStatic = b;
}

void Mass::draw()
{
    // Update the transformation
    mTransform = glm::mat4(glm::translate(mPosition));

    glUseProgram(Mass::getShader()->programID);

    // Send our transformation to the currently bound shader
    GLuint transformID = glGetUniformLocation(Mass::getShader()->programID, "M");
    glUniformMatrix4fv(transformID, 1, GL_FALSE, &mTransform[0][0]);

    glBindVertexArray(vao);
    // (mode, vertex count, type, element array buffer offset)
    glDrawElements(GL_TRIANGLES, 3 * numberOfTriangles, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}


void Mass::setShader(GLShader* shader)
{
    basicShader = shader;
}

GLShader* Mass::getShader()
{
    return basicShader;
}

glm::vec3 Mass::getPosition()
{
    return mPosition;
}

glm::vec3 Mass::getInitialPosition()
{
    return mInitialPosition;
}

glm::vec3 Mass::getVelocity()
{
    return mVelocity;
}

void Mass::setVelocity(glm::vec3 v)
{
    mVelocity = v;
}

void Mass::setGravity(bool b)
{
    gravityActive = b;
}

void Mass::createDebugBox(float xsize, float ysize, float zsize)
{
    GLfloat vertex_array_data[] = {
        //fram
        xsize / 2, -ysize / 2, zsize / 2, 0, 0, 1, 1, 0, //0
        xsize / 2, ysize / 2, zsize / 2, 0, 0, 1, 1, 1, //1
        -xsize / 2, ysize / 2, zsize / 2, 0, 0, 1, 0, 1, //2
        -xsize / 2, -ysize / 2, zsize / 2, 0, 0, 1, 0, 0, //3
        //bak
        xsize / 2, -ysize / 2, -zsize / 2, 0, 0, -1, 0, 0, //4
        xsize / 2, ysize / 2, -zsize / 2, 0, 0, -1, 0, 1, //5
        -xsize / 2, ysize / 2, -zsize / 2, 0, 0, -1, 1, 1, //6
        -xsize / 2, -ysize / 2, -zsize / 2, 0, 0, -1, 1, 0, //7
        //topp
        xsize / 2, ysize / 2, zsize / 2, 0, 1, 0, 1, 0, //8
        xsize / 2, ysize / 2, -zsize / 2, 0, 1, 0, 1, 1, //9
        -xsize / 2, ysize / 2, -zsize / 2, 0, 1, 0, 0, 1, //10
        -xsize / 2, ysize / 2, zsize / 2, 0, 1, 0, 0, 0, //11
        //botten
        xsize / 2, -ysize / 2, zsize / 2, 0, -1, 0, 0, 0, //12
        xsize / 2, -ysize / 2, -zsize / 2, 0, -1, 0, 1, 0, //13
        -xsize / 2, -ysize / 2, -zsize / 2, 0, -1, 0, 1, 1, //14
        -xsize / 2, -ysize / 2, zsize / 2, 0, -1, 0, 0, 1, //15
        //vänster
        -xsize / 2, -ysize / 2, zsize / 2, -1, 0, 0, 1, 0, //16
        -xsize / 2, ysize / 2, zsize / 2, -1, 0, 0, 1, 1, //17
        -xsize / 2, ysize / 2, -zsize / 2, -1, 0, 0, 0, 1, //18
        -xsize / 2, -ysize / 2, -zsize / 2, -1, 0, 0, 0, 0, //19
        //höger
        xsize / 2, -ysize / 2, zsize / 2, 1, 0, 0, 0, 0, //20
        xsize / 2, ysize / 2, zsize / 2, 1, 0, 0, 0, 1, //21
        xsize / 2, ysize / 2, -zsize / 2, 1, 0, 0, 1, 1, //22
        xsize / 2, -ysize / 2, -zsize / 2, 1, 0, 0, 1, 0, //23
    };
    // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    GLuint index_array_data[] = {
        0, 1, 2, //framsida
        2, 3, 0,

        6, 5, 4, //baksida
        4, 7, 6,

        8, 9, 10, //topp
        10, 11, 8,

        14, 13, 12, //botten
        12, 15, 14,

        16, 17, 18, //vänster
        18, 19, 16,

        20, 23, 22, //höger
        22, 21, 20
    };

    vertexarray = vertex_array_data;
    indexarray = index_array_data;

    numberOfVertices = 24;
    numberOfTriangles = 12;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &indexbuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    glBufferData(GL_ARRAY_BUFFER, 8 * numberOfVertices*sizeof(GLfloat), vertexarray, GL_STATIC_DRAW);

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
        3 * numberOfTriangles*sizeof(GLuint), indexarray, GL_STATIC_DRAW);

    // Deactivate (unbind) the VAO and the buffers again.
    // Do NOT unbind the buffers while the VAO is still bound.
    // The index buffer is an essential part of the VAO state.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
