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
	
	void draw();

	glm::vec3 getPosition();

private:

	glm::vec3 position;
	double mass = 1;
	std::vector<Vertex> connectedMasses;

	GLShader* basicShader;
	

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