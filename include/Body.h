#ifndef BODY_H_
#define BODY_H_

#include <glm/glm.hpp>
#include <vector>
#include <iterator>
#include <memory>

#include "Mass.h"

class Body
{
public:
  Body();
  ~Body();

  void draw();
  void move();
  void toggleDebug() {mDebug = !mDebug;}

private:
  bool mDebug = true;
  std::vector<std::shared_ptr<Mass>> mMasses;
};

#endif // BODY_H_
