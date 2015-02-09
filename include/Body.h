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
  bool mDebug = false;
  std::vector<Vertex*> mVertices;
};

#endif // BODY_H_
