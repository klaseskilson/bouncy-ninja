#ifndef BODY_H_
#define BODY_H_

#include <glm/glm.hpp>
#include <vector>

#include "Vertex.h"

class Body
{
public:
  Body();
  ~Body();

  void draw();
  void toggleDebug() {mDebug = !mDebug;}

private:
  bool mDebug = true;
  std::vector<std::shared_ptr<Vertex>> mVertices;
};

#endif // BODY_H_
