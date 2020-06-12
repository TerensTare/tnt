// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs/RigidBody.hpp"

tnt::PhysicsComponent::PhysicsComponent(
    float const &mass, tnt::Vector const &maxVel_,
    tnt::Vector const &maxAccel) noexcept
    : invMass{1 / mass}, velocity{VECTOR_ZERO}, maxVelocity{maxVel_},
      acceleration{VECTOR_DOWN * 10.f}, maxAcceleration{maxAccel} {}

void tnt::PhysicsComponent::setMass(float const &mass) { invMass = (1 / mass); }
float tnt::PhysicsComponent::getMass() const noexcept(noexcept(invMass > 0.f)) { return (1 / invMass); }

tnt::Vector tnt::PhysicsComponent::getVelocity() const noexcept { return velocity; }
tnt::Vector tnt::PhysicsComponent::getAcceleration() const noexcept { return acceleration; }

tnt::Vector tnt::PhysicsComponent::getMaxVelocity() const noexcept { return maxVelocity; }
tnt::Vector tnt::PhysicsComponent::getMaxAcceleration() const noexcept { return maxAcceleration; }

tnt::Vector tnt::PhysicsComponent::getDirection() const
{
    return velocity.Normalized();
}

float tnt::PhysicsComponent::getSpeed() const noexcept { return velocity.Magnitude(); }
float tnt::PhysicsComponent::getMaxSpeed() const noexcept { return maxVelocity.Magnitude(); }

void tnt::PhysicsComponent::applyForce(tnt::Vector const &force) noexcept(noexcept(invMass > 0.f))
{
    acceleration += (force * invMass);
    acceleration = (acceleration < maxAcceleration) ? acceleration : maxAcceleration;
}

void tnt::PhysicsComponent::doPhysics(long long time_) noexcept
{
    velocity += (acceleration * time_);
    velocity = (velocity < maxVelocity) ? velocity : maxVelocity;
}

tnt::RigidBody::RigidBody(float const &mass) noexcept
    : physics{add<PhysicsComponent>(
          mass, tnt::Vector{50.f, 50.f}, tnt::Vector{10.f, 10.f})} {}

tnt::RigidBody::~RigidBody() noexcept
{
    if (physics != nullptr)
    {
        delete physics;
        physics = nullptr;
    }
}

tnt::PhysicsComponent *tnt::RigidBody::getPhysics() const noexcept { return physics; }

void tnt::RigidBody::doPhysics(long long time_) noexcept
{
    physics->doPhysics(time_);
    position += (physics->getVelocity() * time_);
}