#include "Body.h"
bool Body::mDebug = true;

Body::Body()
{
	mVertices.push_back(std::shared_ptr<Vertex>(new Vertex));
	mVertices.push_back(std::shared_ptr<Vertex>(new Vertex(glm::vec3(2.0f,0.0f,0.0f))));
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

