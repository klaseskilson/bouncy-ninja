#ifndef BOUNCY_HELPERS_H_
#define BOUNCY_HELPERS_H_

#include <glm/glm.hpp>
#include <iostream>

#define PI 3.14159265359

std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec);
std::ostream &operator<< (std::ostream &out, const glm::vec2 &vec);
std::ostream &operator<< (std::ostream &out, const glm::mat4 &mat);

#endif //BOUNCY_HELPERS_H_
