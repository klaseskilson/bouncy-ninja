#ifndef VERTEX_H_
#define VERTEX_H_

#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <GL/glew.h> 
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "GLShader.h"

class Vertex{
public:

	Vertex();
	Vertex(glm::vec3 pos);
	~Vertex();
	
	//Render debug cubes for debug mode
	void draw();

	//Set shader, ONLY USED WHEN IN DEBUG MODE.
	static void setShader(GLShader* shader);

	//Returns vertex position
	glm::vec3 getPosition();

private:

	//Initializes each vertex with a box for debug purposes
	void createDebugBox(float xsize, float ysize, float zsize);

	glm::vec3 position;
	double mass = 1;
	std::vector<Vertex> connectedMasses;

	static GLShader* basicShader;
	

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