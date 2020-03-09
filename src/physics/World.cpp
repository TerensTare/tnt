// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "physics/World.hpp"

void tnt::World::startFrame()
{
    auto reg{first};
    while (reg != nullptr)
    {
        reg->particle->clearAccumulator();
        reg = reg->next;
    }
}

unsigned tnt::World::generateContacts()
{
    unsigned limit{maxContacts};
    Contact *next{contacts};

    ContactGenRegistration *reg{firstContactGen};
    while (reg)
    {
        unsigned used{reg->gen->addContact(next, limit)};
        limit -= used;
        next += used;
        if (limit <= 0)
            break;
        reg = reg->next;
    }

    return maxContacts - limit;
}

void tnt::World::integrate(float duration)
{
    Registration *reg{first};
    while (reg)
    {
        reg->particle->integrate(duration);
        reg = reg->next;
    }
}

void tnt::World::runPhysics(float duration)
{
    registry.update(duration);
    integrate(duration);
    unsigned usedContacts{generateContacts()};

    if (true)
        resolver.setIterations(usedContacts * 2);
    resolver.resolve(contacts, usedContacts, duration);
}