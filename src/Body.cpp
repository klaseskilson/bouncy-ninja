#include "Body.h"

Body::Body()
{
  mVertices.push_back(std::shared_ptr<Vertex>(new Vertex));
}

Body::~Body()
{
  mVertices.clear();
}

void Body::draw()
{
  if (mDebug)
  {
    for (std::vector<std::shared_ptr<Vertex>>::iterator it = mVertices.begin(); it != mVertices.end(); ++it)
    {
      (*it)->draw();
    }
  }
}

