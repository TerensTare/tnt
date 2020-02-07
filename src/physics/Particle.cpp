// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "physics/Particle.hpp"

void tnt::Particle::integrate(real duration)
{
    assert(duration > 0);
    pos += (velocity * duration);

    auto res{acceleration};
    res += (forceAccumulator * inverseMass);
    velocity += (res * duration);
    velocity *= pow(damping, duration);
}