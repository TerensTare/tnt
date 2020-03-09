#ifndef WORLD_HPP
#define WORLD_HPP

#include "physics/ForceGenerator.hpp"
#include "physics/Contact.hpp"

// TODO(maybe): rename this to ParticleWorld.hpp

namespace tnt
{
class World
{
public:
    World(unsigned maxContacts, unsigned iterations = 0);

    void startFrame();
    void integrate(float duration);
    void runPhysics(float duration);

    unsigned generateContacts();

protected:
    struct Registration
    {
        Particle *particle;
        Registration *next;
    };
    struct ContactGenRegistration
    {
        ContactGenerator *gen;
        ContactGenRegistration *next;
    };

    unsigned maxContacts;
    Registration *first;
    ForceRegistry registry;
    ContactResolver resolver;
    ContactGenRegistration *firstContactGen;
    Contact *contacts;
};
} // namespace tnt

#endif //!WORLD_HPP