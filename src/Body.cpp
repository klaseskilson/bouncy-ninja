#include "Body.h"

Body::Body()
{
	mMasses.push_back(std::shared_ptr<Mass>(new Mass));
	mMasses.push_back(std::shared_ptr<Mass>(new Mass(glm::vec3(2.0f,0.0f,0.0f))));
}

Body::~Body()
{
  mMasses.clear();
}

void Body::draw()
{
  if (mDebug)
  {
    for (std::vector<std::shared_ptr<Mass>>::iterator it = mMasses.begin(); it != mMasses.end(); ++it)
    {
      (*it)->draw();
    }
  }
}

