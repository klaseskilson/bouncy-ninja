#include "Body.h"

Body::Body()
{
  mVertices.push_back(new Vertex());
}

Body::~Body()
{

}

void Body::draw()
{
  if (mDebug)
  {
    for (std::vector<Vertex*>::iterator it = mVertices.begin(); it != mVertices.end(); ++it)
    {
      (*it)->draw();
    }
  }
}

