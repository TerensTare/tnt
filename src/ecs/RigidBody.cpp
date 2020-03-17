// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "RigidBody.hpp"

void tnt::RigidBody::setMass(float mass) { invMass = (1 / mass); }
float tnt::RigidBody::getMass() const noexcept(noexcept(invMass > 0.f)) { return 1 / invMass; }

tnt::Vector tnt::RigidBody::getVelocity() const noexcept { return velocity; }
tnt::Vector tnt::RigidBody::getAcceleration() const noexcept { return acceleration; }

void tnt::RigidBody::applyForce(tnt::Vector const &force) noexcept(noexcept(invMass > 0.f))
{
    acceleration += (force * invMass);
}

void tnt::RigidBody::Update()
{
    velocity += acceleration;
    pos += velocity;
}