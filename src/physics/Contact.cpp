// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "physics/Contact.hpp"

void tnt::Contact::resolve(float duration)
{
    resolveVel(duration);
    resolveInterpenetration(duration);
}

float tnt::Contact::calcSeparatingVelocity() const
{
    Vector relativeVel{particles[0]->getVelocity()};
    if (particles[1])
        relativeVel -= particles[1]->getVelocity();
    return (relativeVel.x * contactNormal.x + relativeVel.y * contactNormal.y);
}

void tnt::Contact::resolveVel(float duration)
{
    float sepVel{calcSeparatingVelocity()};
    if (sepVel > 0)
        return;
    float newSepVel{-sepVel * restitution};

    Vector accCausedVel{particles[0]->getAcceleration()};
    if (particles[1])
        accCausedVel -= particles[1]->getAcceleration();
    float accCausedSepVel{(accCausedVel.x * contactNormal.x + accCausedVel.y * contactNormal.y) * duration};

    if (accCausedSepVel < 0)
    {
        newSepVel += restitution * accCausedSepVel;
        if (newSepVel < 0)
            newSepVel = 0;
    }

    float deltaVel{newSepVel - sepVel};
    float totalInvMass{1 / particles[0]->getMass()};
    if (particles[1])
        totalInvMass += (1 / particles[1]->getMass());
    float impulse{deltaVel / totalInvMass};

    Vector impulsePerMass{contactNormal * impulse};
    particles[0]->setVelocity(
        particles[0]->getVelocity() +
        impulsePerMass / particles[0]->getMass());

    if (particles[1])
        particles[1]->setVelocity(
            particles[1]->getVelocity() -
            impulsePerMass * particles[1]->getMass());
}

void tnt::Contact::resolveInterpenetration(float duration)
{
    if (penetration <= 0)
        return;

    float totalInvMass{1 / particles[0]->getMass()};
    if (particles[1])
        totalInvMass += (1 / particles[1]->getMass());

    if (totalInvMass <= 0)
        return;

    Vector movePerMass{contactNormal * (-penetration / totalInvMass)};
    particles[0]->setPosition(
        particles[0]->getPosition() + movePerMass / particles[0]->getMass());
    if (particles[1])
        particles[1]->setPosition(
            particles[1]->getPosition() + movePerMass / particles[1]->getMass());
}

void tnt::ContactResolver::resolve(tnt::Contact *contacts, unsigned int number, float duration)
{
    iterationsUsed = 0;
    while (iterationsUsed < iterations)
    {
        float max{0};

        unsigned maxIdx{number};
        for (unsigned i{0}; i < number; ++i)
        {
            float sepVel{contacts[i].calcSeparatingVelocity()};
            if (sepVel < max)
            {
                max = sepVel;
                maxIdx = i;
            }
        }

        contacts[maxIdx].resolve(duration);
        ++iterationsUsed;
    }
}