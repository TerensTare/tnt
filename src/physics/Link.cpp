// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "physics/Link.hpp"

float tnt::BodiesLink::getLength() const noexcept
{
    Vector relPos{particles[0]->getPosition() - particles[1]->getPosition()};
    return relPos.Magnitude();
}

unsigned tnt::CableLink::fillContact(tnt::Contact *contact, unsigned limit) const
{
    float length{getLength()};
    if (length < maxLength)
        return 0;
    contact->particles[0] = particles[0];
    contact->particles[1] = particles[1];

    Vector normal{particles[1]->getPosition() - particles[0]->getPosition()};
    normal.Normalize();
    contact->contactNormal = normal;
    contact->penetration = length - maxLength;
    contact->restitution = restitution;

    return 1;
}

unsigned tnt::RodLink::fillContact(tnt::Contact *contact, unsigned int limit) const
{
    float cLen{getLength()};
    if (cLen == length)
        return 0;
    contact->particles[0] = particles[0];
    contact->particles[1] = particles[1];

    Vector normal{particles[1]->getPosition() - particles[0]->getPosition()};
    normal.Normalize();
    if (cLen > length)
    {
        contact->contactNormal = normal;
        contact->penetration = cLen - length;
    }
    else
    {
        contact->contactNormal = -normal;
        contact->penetration = length - cLen;
    }

    contact->restitution = 0;
    return 1;
}