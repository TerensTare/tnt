#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Vector.hpp"

namespace tnt
{
struct Particle
{
    void integrate(real duration);

    real damping;
    real inverseMass; // this way mass can't be 0

    Vector pos;
    Vector velocity;
    Vector acceleration;
    Vector forceAccumulator;
};
} // namespace tnt

#endif //!PARTICLE_HPP