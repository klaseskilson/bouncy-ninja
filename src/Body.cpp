#include "Body.h"
bool Body::mDebug = true;

Body::Body()
{
  Mass* tmpMass = new Mass(glm::vec3(-1.0f,0.0f,1.0f));
  Mass* tmpMass2 = new Mass(glm::vec3(1.0f,0.0f,1.0f));
  Mass* tmpMass3 = new Mass(glm::vec3(1.0f, 0.0f, -1.0f));
  Mass* tmpMass4 = new Mass(glm::vec3(-1.5f, 0.0f, -1.0f));

  tmpMass4->setVelocity(glm::vec3(-1.0f));

  tmpMass->connectMass(tmpMass2);
  tmpMass->connectMass(tmpMass4);
  tmpMass->connectMass(tmpMass3);
  tmpMass2->connectMass(tmpMass3);
  tmpMass4->connectMass(tmpMass3);
  tmpMass4->connectMass(tmpMass2);

  mMasses.push_back(std::shared_ptr<Mass>(tmpMass));
  mMasses.push_back(std::shared_ptr<Mass>(tmpMass2));
  mMasses.push_back(std::shared_ptr<Mass>(tmpMass3));
  mMasses.push_back(std::shared_ptr<Mass>(tmpMass4));
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

void Body::update(float timeDelta)
{
  for (std::vector<std::shared_ptr<Mass>>::iterator it = mMasses.begin(); it != mMasses.end(); ++it)
  {
    (*it)->update(timeDelta);
  }
}

