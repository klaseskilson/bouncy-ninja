#ifndef MASS_H_
#define MASS_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "GLShader.h"

class Mass{
public:
	//Constructors
	Mass();
	Mass(glm::vec3 pos);

	//Destructor
	~Mass();

	//Update the simulation
	void update();

	//Render debug cubes for debug mode
	void draw();

	//Set shader, ONLY USED WHEN IN DEBUG MODE.
	static void setShader(GLShader* shader);
	static GLShader* getShader();

	//Returns vertex position
	glm::vec3 getPosition();

	//Initializes each vertex with a box for debug purposes
	void createDebugBox(float xsize, float ysize, float zsize);

private:
  static GLShader* basicShader;

  glm::vec3 mPosition;
  glm::mat4 mTransform;
  double mMass = 1;
  std::vector<Mass> mConnectedMasses;

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
