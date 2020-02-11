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

    clearAccumulator();
}

void tnt::Particle::setDamping(const real &value) noexcept { damping = value; }
real tnt::Particle::getDamping() const noexcept { return damping; }

void tnt::Particle::setMass(const real &mass) noexcept
{
    assert(mass > 0);
    inverseMass = (1 / mass);
}

real tnt::Particle::getMass() const noexcept
{
    assert(inverseMass > 0);
    return (1 / inverseMass);
}

void tnt::Particle::setPosition(const tnt::Vector &position) noexcept { pos = position; }
tnt::Vector tnt::Particle::getPosition() const noexcept { return pos; }

void tnt::Particle::setVelocity(const tnt::Vector &vel) noexcept { velocity = vel; }
tnt::Vector tnt::Particle::getVelocity() const noexcept { return velocity; }

void tnt::Particle::setAcceleration(const tnt::Vector &accel) noexcept { acceleration = accel; }
tnt::Vector tnt::Particle::getAcceleration() const noexcept { return acceleration; }

void tnt::Particle::addForce(const tnt::Vector &force) noexcept { forceAccumulator += force; }
void tnt::Particle::clearAccumulator() noexcept { forceAccumulator = {real0, real0}; }