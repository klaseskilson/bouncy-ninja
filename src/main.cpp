#ifdef WIN32
#define GLEW_STATIC
#endif

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <glm/glm.hpp>
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include "bouncy_helpers.h"

#include "Body.h"
#include "Boundary.h"
#include "Vertex.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Body* gBody;

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

  // setup key events
  glfwSetKeyCallback(window, key_callback);

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable(GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

  gBody = new Body();

  Boundary floor = Boundary(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -0.9f, 1.0f));

  printf("\nLet's get ready to render!\n\n");
  while (!glfwWindowShouldClose(window)) {
    // wipe the drawing surface clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glUseProgram(shader_programme);

    gBody->draw();

	  // update other events like input handling
	  glfwPollEvents();
	  // put the stuff we've been drawing onto the display
	  glfwSwapBuffers(window);
  }

  delete gBody;
  // close GL context and any other GLFW resources
  glfwTerminate();
  return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
  if (key == GLFW_KEY_D && action == GLFW_PRESS)
  {
    std::cout << "Toggling debug!" << std::endl;
    gBody->toggleDebug();
  }
}
