#ifdef WIN32
#define GLEW_STATIC
#endif

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <glm/glm.hpp>
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "bouncy_helpers.h"
#include "GLShader.h"
#include "Vertex.h"
#include "Boundary.h"

int main()
{
  printf("Initiating...\n");
  // start GL context and O/S window using the GLFW helper library
  if (!glfwInit())
  {
    fprintf(stderr, "ERROR: could not start GLFW3\n");
    return -1;
  }

  #ifdef __APPLE__
    // force opengl verison for os x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  #endif

  GLFWwindow* window = glfwCreateWindow(640, 480, "Bouncy Ninja", NULL, NULL);
  if (!window)
  {
    fprintf(stderr, "ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable(GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"


  Vertex cube = Vertex();


  GLShader basicShader("../shaders/main.vert", "../shaders/main.frag");
  glUseProgram(basicShader.programID);

  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  // Camera matrix
  glm::mat4 View = glm::lookAt(
	  glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
	  glm::vec3(0, 0, 0), // and looks at the origin
	  glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	  );
  glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model !
  // Our ModelViewProjection : multiplication of our 3 matrices
  glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

  GLuint MatrixID = glGetUniformLocation(basicShader.programID, "MVP");
  // Send our transformation to the currently bound shader,
  // in the "MVP" uniform
  // For each model you render, since the MVP will be different (at least the M part)
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);



  printf("\nLet's get ready to render!\n\n");
  while (!glfwWindowShouldClose(window)) {
	  // wipe the drawing surface clear
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  //glUseProgram(shader_programme);

	  cube.draw();

	  // update other events like input handling
	  glfwPollEvents();
	  // put the stuff we've been drawing onto the display
	  glfwSwapBuffers(window);
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  return 0;
}
