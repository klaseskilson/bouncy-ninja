#ifndef BODY_H_
#define BODY_H_

#include <glm/glm.hpp>
#include <vector>
#include <iterator>
#include <memory>

#include "Mass.h"
#include "Boundary.h"
#include "vboindexer.hpp"
#include "objloader.hpp"

class Body
{
public:
    Body();
    ~Body();

    void draw();
    void update(float timeDelta);

    // return the position of the center-most mass
    glm::vec3 getCenter();

    // boundaries
    void addBoundary(std::shared_ptr<Boundary>);
    std::vector<std::shared_ptr<Boundary>> getBoundaries();

    // object creation
    void createBox(int xSize, int ySize, int zSize, float massDistance, glm::vec3 sPoint);
    void createRope(int length, glm::vec3 startPoint);
    void loadObj(const char * path);

    // interaction
    void hit();
    static void toggleDebug() {mDebug = !mDebug;}

    // shader handeling
    static void setShader(GLShader* shader);
    static GLShader* getShader();

private:
    void updateGL();
    void updateVertices();

    static bool mDebug;
    std::vector<std::shared_ptr<Mass>> mMasses;
    std::vector<std::shared_ptr<Boundary>> mBoundaries;

    int xSize, ySize, zSize;
    //GL variables
    static GLShader* basicShader;

    std::vector<unsigned short> indices;
    std::vector<GLfloat> vertexarray;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    GLuint vao;
    int numberOfVertices;
    int numberOfTriangles;

    GLuint vertexbuffer;
    GLuint normalbuffer;
    GLuint uvbuffer;
    GLuint indexbuffer;

    //GLfloat *vertexarray;
    GLuint *indexarray;
};

#endif // BODY_H_
