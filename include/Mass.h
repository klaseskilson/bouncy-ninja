#ifndef MASS_H_
#define MASS_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "bouncy_helpers.h"
#include "GLShader.h"

class Mass{
public:
	//Constructors
	Mass();
	Mass(glm::vec3 pos);

	//Destructor
	~Mass();

	//Update the simulation
	void update(float timeDelta);

	//Render debug cubes for debug mode
	void draw();

	//Connect with another mass
	void connectMass(Mass* m);

	//Set shader, ONLY USED WHEN IN DEBUG MODE.
	static void setShader(GLShader* shader);
	static GLShader* getShader();

	//Returns vertex position
	glm::vec3 getPosition();

	glm::vec3 getVelocity();
	//Set velocity
	void setVelocity(glm::vec3 v);


	//Initializes each vertex with a box for debug purposes
	void createDebugBox(float xsize, float ysize, float zsize);

	std::vector<Mass*> mConnectedMasses;

private:
  static GLShader* basicShader;

  glm::vec3 mVelocity;

  glm::vec3 mPosition;
  glm::mat4 mTransform;
  float mMass = 1;

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
