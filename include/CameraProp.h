#ifndef CAMERA_PROP_H_
#define CAMERA_PROP_H_

#include <glm/glm.hpp>

struct CameraProp
{
    glm::vec3 obj = glm::vec3(0.0f);
    glm::vec4 cam = glm::vec4(7.0f, 0.0f, 0.0f, 1.0f);

    float xMouse, yMouse;
};

#endif //CAMERA_PROP_H_
