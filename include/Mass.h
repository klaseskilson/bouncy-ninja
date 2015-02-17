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

// #include "Body.h"
#include "bouncy_helpers.h"
#include "GLShader.h"
#include "Boundary.h"

class Mass
{
public:
    //Constructors
    Mass();
    Mass(glm::vec3 pos, float spring = 100.0f, float damping = 45.9f);

    //Destructor
    ~Mass();

    //Update the simulation
    void update(float timeDelta, std::vector<std::shared_ptr<Boundary>> boundaries);

    //Render debug cubes for debug mode
    void draw();

    //Connect with another mass
    void connectMass(std::shared_ptr<Mass> m);

    //Set shader, ONLY USED WHEN IN DEBUG MODE.
    static void setShader(GLShader* shader);

    //toggle if gravity is active
    static void setGravity(bool b);
    void setStatic(bool b);

    static GLShader* getShader();

    //Returns vertex position
    glm::vec3 getPosition();
    glm::vec3 getInitialPosition();

    glm::vec3 getVelocity();
    //Set velocity
    void setVelocity(glm::vec3 v);


    //Initializes each vertex with a box for debug purposes
    void createDebugBox(float xsize, float ysize, float zsize);

    void explicitEuler(glm::vec3 force, float h);
    void implicitEuler(glm::vec3 force, float h);
    void rungeKutta(glm::vec3 force, float h);

    std::vector<std::shared_ptr<Mass>> mConnectedMasses;

private:
    static GLShader* basicShader;
    static bool gravityActive;

    bool mIsStatic = false;

    glm::vec3 mVelocity;

    glm::vec3 mPosition;
    glm::vec3 mInitialPosition;
    glm::mat4 mTransform;

    float mMass = 1.0f;
    float mSpringConstant, mDampingConstant;

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
