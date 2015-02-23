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
    void move();

    void addBoundary(std::shared_ptr<Boundary>);
    std::vector<std::shared_ptr<Boundary>> getBoundaries();

    void createCube(int nMasses, float massDistance, glm::vec3 sPoint);
    void loadObj(const char * path);
    
    static void setShader(GLShader* shader);
    static GLShader* getShader();

    static void toggleDebug() {mDebug = !mDebug;}

private:
    void updateGL();
    void updateVertices();


    static bool mDebug;
    std::vector<std::shared_ptr<Mass>> mMasses;
    std::vector<std::shared_ptr<Boundary>> mBoundaries;

    int boxSize;
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
