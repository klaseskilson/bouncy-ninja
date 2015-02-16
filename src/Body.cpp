#include "Body.h"
bool Body::mDebug = true;

Body::Body()
{
    Mass* tmpMass = new Mass(glm::vec3(-1.0f,1.0f,1.0f));
    Mass* tmpMass2 = new Mass(glm::vec3(1.0f,1.0f,1.0f));
    Mass* tmpMass3 = new Mass(glm::vec3(1.0f, 1.0f, -1.0f));
    Mass* tmpMass4 = new Mass(glm::vec3(-1.5f, 1.0f, -1.0f));
    Mass* tmpMass5 = new Mass(glm::vec3(-1.0f, -1.0f, 1.0f));
    Mass* tmpMass6 = new Mass(glm::vec3(1.0f, -1.0f, 1.0f));
    Mass* tmpMass7 = new Mass(glm::vec3(1.0f, -1.0f, -1.0f));
    Mass* tmpMass8 = new Mass(glm::vec3(-1.5f, -1.0f, -1.0f));

    //tmpMass4->setVelocity(glm::vec3(-8.0f,0.0f,0.0f));

    tmpMass->setStatic(true);

    tmpMass->connectMass(tmpMass2);
    tmpMass->connectMass(tmpMass3);
    tmpMass->connectMass(tmpMass4);
    tmpMass->connectMass(tmpMass5);
    tmpMass->connectMass(tmpMass6);
    tmpMass->connectMass(tmpMass7);
    tmpMass->connectMass(tmpMass8);

    tmpMass2->connectMass(tmpMass3);
    tmpMass2->connectMass(tmpMass4);
    tmpMass2->connectMass(tmpMass5);
    tmpMass2->connectMass(tmpMass6);
    tmpMass2->connectMass(tmpMass7);
    tmpMass2->connectMass(tmpMass8);

    tmpMass3->connectMass(tmpMass4);
    tmpMass3->connectMass(tmpMass5);
    tmpMass3->connectMass(tmpMass6);
    tmpMass3->connectMass(tmpMass7);
    tmpMass3->connectMass(tmpMass8);

    tmpMass4->connectMass(tmpMass5);
    tmpMass4->connectMass(tmpMass6);
    tmpMass4->connectMass(tmpMass7);
    tmpMass4->connectMass(tmpMass8);

    tmpMass5->connectMass(tmpMass6);
    tmpMass5->connectMass(tmpMass7);
    tmpMass5->connectMass(tmpMass8);

    tmpMass6->connectMass(tmpMass7);
    tmpMass6->connectMass(tmpMass8);

    tmpMass7->connectMass(tmpMass8);

    mMasses.push_back(std::shared_ptr<Mass>(tmpMass));
    mMasses.push_back(std::shared_ptr<Mass>(tmpMass2));
    mMasses.push_back(std::shared_ptr<Mass>(tmpMass3));
    mMasses.push_back(std::shared_ptr<Mass>(tmpMass4));
    mMasses.push_back(std::shared_ptr<Mass>(tmpMass5));
    mMasses.push_back(std::shared_ptr<Mass>(tmpMass6));
    mMasses.push_back(std::shared_ptr<Mass>(tmpMass7));
    mMasses.push_back(std::shared_ptr<Mass>(tmpMass8));
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

