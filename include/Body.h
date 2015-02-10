#ifndef BODY_H_
#define BODY_H_

#include <glm/glm.hpp>
#include <vector>
#include <iterator>
#include <memory>

#include "Vertex.h"

class Body
{
public:
  Body();
  ~Body();

  void draw();
  void move();
  static void toggleDebug() {mDebug = !mDebug;}

private:
  static bool mDebug;
  std::vector<std::shared_ptr<Vertex>> mVertices;
};

#endif // BODY_H_
