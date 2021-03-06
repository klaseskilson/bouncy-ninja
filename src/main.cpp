#ifdef WIN32
#define GLEW_STATIC
#endif

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <sstream>
#include <iostream>

#include "bouncy_helpers.h"

#include "Body.h"
#include "Boundary.h"
#include "CameraProp.h"
#include "Mass.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void updateCamera();

bool gRunSimulation = true, gDrawPolygons = false;

CameraProp gCam;

int main()
{
    std::cout << "Choose spring constant: ";
    std::cin >> Mass::spring;
    std::cout << "Choose damping constant: ";
    std::cin >> Mass::damping;

    printf("Initiating...\n");
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return -1;
    }

    // force opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        // force opengl verison for os x
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif
    
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Bouncy Ninja", glfwGetPrimaryMonitor(), NULL);
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
    //setup mouse event function
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // setup scroll callback
    glfwSetScrollCallback(window, scroll_callback);


    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"


    GLShader* simpleShader = new GLShader("../shaders/simple.vert", "../shaders/simple.frag");
    GLShader* floorShader = new GLShader("../shaders/floor.vert", "../shaders/floor.frag");
    Mass::setShader(simpleShader);
    Mass::setGravity(true);

    Boundary::setShader(floorShader);

    updateCamera();

    Body theBody = Body();

    // create our floor and set it up
    std::shared_ptr<Boundary> floor(new Boundary(glm::vec3(-10.0f, -10.0f, -10.0f), glm::vec3(10.0f, -2.9f, 10.0f)));
    theBody.addBoundary(floor);
    // create wall and set it upp
    std::shared_ptr<Boundary> wall(new Boundary(glm::vec3(-11.0f, -10.0f, -10.0f), glm::vec3(-10.0f, 10.0f, 10.0f)));
    theBody.addBoundary(wall);

    float timeDelta = glfwGetTime();
    float cappedStep = 0.01f;
    float lastFpsUpdate = 0.0f;
    int nbrOfFrames = 0;

    printf("\nLet's get ready to render!\n\n");
    glfwSwapInterval(0);
    while (!glfwWindowShouldClose(window))
    {
        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (gDrawPolygons)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        //Simulation update
        if (gRunSimulation)
        {
            if ((glfwGetTime() - timeDelta) > cappedStep)
            {
                theBody.update(cappedStep);
            }
            else
            {
                theBody.update(glfwGetTime() - timeDelta);
            }

            timeDelta = glfwGetTime();
        }

        updateCamera();

        //DRAWING CALLS
        floor->draw();
        wall->draw();
        theBody.draw();

        gCam.obj = theBody.getCenter();

        //FPS counter
        nbrOfFrames++;
        //Only update once every second
        if (glfwGetTime() - lastFpsUpdate > 1.0f)
        {
            std::stringstream ss;
            ss << "Bouncy Ninja - FPS: " << double(nbrOfFrames) / (glfwGetTime() - lastFpsUpdate);
            glfwSetWindowTitle(window, ss.str().c_str());
            lastFpsUpdate = glfwGetTime();
            nbrOfFrames = 0;
        }

        // update other events like input handling
        glfwPollEvents();
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
    }
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
        std::cout << "Toggling debug! Press D to toggle again." << std::endl;
        Body::toggleDebug();
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        gRunSimulation = !gRunSimulation;
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        gDrawPolygons = !gDrawPolygons;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // std::cout << "scroll: " << yoffset << std::endl;
    gCam.cam.x += yoffset;
    gCam.cam.x = fmax(gCam.cam.x, 0.5);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    gCam.xMouse = xpos;
    gCam.yMouse = ypos;
}

void updateCamera()
{
    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 rotationMat = glm::eulerAngleYZ(gCam.xMouse*0.01f, gCam.yMouse*0.01f);
    glm::vec4 cameraPosition = gCam.cam * rotationMat;
    glm::vec3 cameraPos = glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z);

    glm::mat4 View = glm::lookAt(
        gCam.obj + cameraPos, // Camera starts at (7,0,0), in World Space
        gCam.obj, // and looks at the center of the object
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
        );

    glUseProgram(Mass::getShader()->programID);
    GLuint MatrixIDV = glGetUniformLocation(Mass::getShader()->programID, "V");
    GLuint MatrixIDP = glGetUniformLocation(Mass::getShader()->programID, "P");

    glUniformMatrix4fv(MatrixIDV, 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(MatrixIDP, 1, GL_FALSE, &Projection[0][0]);

    glUseProgram(Boundary::getShader()->programID);
    GLuint FMatrixIDV = glGetUniformLocation(Boundary::getShader()->programID, "V");
    GLuint FMatrixIDP = glGetUniformLocation(Boundary::getShader()->programID, "P");

    glUniformMatrix4fv(FMatrixIDV, 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(FMatrixIDP, 1, GL_FALSE, &Projection[0][0]);
}
