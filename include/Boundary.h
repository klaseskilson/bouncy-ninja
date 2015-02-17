#ifndef BOUNDARY_H_
#define BOUNDARY_H_

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <math.h>

#include "bouncy_helpers.h"
#include "GLShader.h"

class Boundary
{
public:
    /**
     * create a boundary with two opposite corners in a and b
     *
     * glm::vec3  a   the a corner
     * glm::vec3  b   the b corner
     *
     * like this:
     *      +------+ <-(b)      |y
     *    .'|    .'|            | /z
     *  +---+--+'  |            |/____x
     *  |   +--+---+
     *  | .'   | .'
     *  +------+'
     *  ^(a)
     */
    Boundary(glm::vec3, glm::vec3);

    // destructor
    ~Boundary();

    void setShader(GLShader*);

    void draw();

    float getTop();
    float getBottom();

    void getProperPosition(glm::vec3 &pos, glm::vec3 oldPos, glm::vec3 &vel);

private:
    // our corners
    glm::vec3 mA, mB;

    //GL variables
    GLShader* mShader;
    GLuint mGlVao;
    int mGlNumberOfVertices;
    int mGlNumberOfTriangles;
    GLuint mGlVertexBuffer;
    GLuint mGlIndexBuffer;
    GLfloat *mGlVertexArray;
    GLuint *mGlIndexArray;
};

#endif // BOUNDARY_H_
