#ifndef MASS_H_
#define MASS_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <memory>

#include "bouncy_helpers.h"
#include "GLShader.h"
#include "Boundary.h"

class Mass
{
public:
    //Constructors
    Mass();

    Mass(glm::vec3 pos, float spring = Mass::spring, float damping = Mass::damping);

    //Destructor
    ~Mass();

    // Update the simulation
    void update(float timeDelta, std::vector<std::shared_ptr<Boundary>> boundaries);

    // Render debug cubes for debug mode
    void draw();

    // Connect with another mass
    void connectMass(std::shared_ptr<Mass> m);

    // Set shader, ONLY USED WHEN IN DEBUG MODE.
    static void setShader(GLShader* shader);

    // toggle if gravity is active
    static void setGravity(bool b);
    void setStatic(bool b);

    static GLShader* getShader();

    // Returns vertex' positions
    glm::vec3 getPosition();
    glm::vec3 getInitialPosition();

    // Get/Set velocity
    glm::vec3 getVelocity();
    void setVelocity(glm::vec3 v);


    //Initializes each vertex with a box for debug purposes
    void createDebugBox(float xsize, float ysize, float zsize);

    glm::vec3 accel(glm::vec3, glm::vec3);

    void explicitEuler(float h);
    void implicitEuler(float h);
    void rungeKutta(float h);

    std::vector<Mass*> mConnectedMasses;

    static float spring;
    static float damping;

private:
    static GLShader* basicShader;
    static bool gravityActive;

    // physics stuff
    float mMass = 1.0f;
    float mSpringConstant, mDampingConstant;

    // decide if point can move
    bool mIsStatic = false;

    // velocity
    glm::vec3 mVelocity;
    // current position
    glm::vec3 mPosition;
    // initial position
    glm::vec3 mInitialPosition;

    // MVP stuff
    glm::mat4 mTransform;

    //GL variables
    GLuint vao;
    int numberOfVertices;
    int numberOfTriangles;
    GLuint vertexbuffer;
    GLuint indexbuffer;
    GLfloat *vertexarray;
    GLuint *indexarray;
};

#endif
