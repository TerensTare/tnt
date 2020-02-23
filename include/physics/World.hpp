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

protected:
    struct Registration
    {
        Particle *particle;
        Registration *next;
    };

    Registration *first;
    ForceRegistry registry;
    ContactResolver resolver;

    struct ContactGenRegistration
    {
        ContactGenerator *gen;
        ContactGenRegistration *next;
    };
};
} // namespace tnt

#endif //!WORLD_HPP