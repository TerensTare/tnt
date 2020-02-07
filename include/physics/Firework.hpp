#ifndef FIREWORK_HPP
#define FIREWORK_HPP

#include "physics/Particle.hpp"

namespace tnt
{
struct FireworkRule
{
    void create(Firework *firework, Firework const *parent = nullptr) const;

    real minAge;
    real maxAge;
    real damping;

    unsigned type;
    unsigned payloadCount;

    Vector minVelocity;
    Vector maxVelocity;

    struct Payload // the new firework to create.
    {
        unsigned type;
        unsigned count;
    };

    Payload *payloads;
};

struct Firework : public Particle
{
    bool update(real duration);

    unsigned type;
    real age;
};
} // namespace tnt

#endif //!FIREWORK_HPP