#ifndef BOUNDARY_H_
#define BOUNDARY_H_

#include <glm/glm.hpp>
#include <vector>
#include <iostream>


class Boundary
{
public:
  /**
   * create a boundary with two opposite corners in first and second
   *
   * glm::vec3  first   the first corner
   * glm::vec3  second  the second corner
   *
   * like this:
   *      +------+ (b)
   *    .'|    .'|
   *  +---+--+'  |
   *  |   +--+---+
   *  | .'   | .'
   *  +------+'
   *  ^(a)
   */
  Boundary(glm::vec3 a, glm::vec3 b)
    : first(a), second(b) {}

  // destructor
  ~Boundary() {}

private:
  glm::vec3 first, second;
};

#endif // BOUNDARY_H_
