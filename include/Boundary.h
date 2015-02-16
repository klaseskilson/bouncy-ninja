#ifndef BOUNDARY_H_
#define BOUNDARY_H_

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <math.h>


class Boundary
{
public:
  /**
   * create a boundary with two opposite corners in a and b
   *
   * glm::vec3  a   the a corner
   * glm::vec3  b   the b corner
   *
   * like this:
   *      +------+ <-(b)
   *    .'|    .'|
   *  +---+--+'  |
   *  |   +--+---+
   *  | .'   | .'
   *  +------+'
   *  ^(a)
   */
  Boundary(glm::vec3 a, glm::vec3 b)
    : a(a), b(b) {}

  // destructor
  ~Boundary() {}

  float getTop();
  float getBottom();

private:
  glm::vec3 a, b;
};

#endif // BOUNDARY_H_
