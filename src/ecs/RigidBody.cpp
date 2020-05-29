// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs/RigidBody.hpp"

tnt::PhysicsComponent::PhysicsComponent(float const &mass, Rectangle const &collision_box)
    : invMass{1 / mass}, velocity{VECTOR_ZERO}, acceleration{VECTOR_ZERO},
      collisionBox{collision_box} {}

tnt::PhysicsComponent::PhysicsComponent(float const &mass, float x, float y, float &w, float &h)
    : invMass{1 / mass}, velocity{VECTOR_ZERO}, acceleration{VECTOR_ZERO},
      collisionBox{x, y, w, h} {}

void tnt::PhysicsComponent::setMass(float const &mass) { invMass = (1 / mass); }
float tnt::PhysicsComponent::getMass() const noexcept(noexcept(invMass > 0.f)) { return (1 / invMass); }

tnt::Vector tnt::PhysicsComponent::getVelocity() const noexcept { return velocity; }
tnt::Vector tnt::PhysicsComponent::getAcceleration() const noexcept { return acceleration; }

tnt::Rectangle tnt::PhysicsComponent::getCollisionBox() const noexcept { return collisionBox; }

void tnt::PhysicsComponent::applyForce(tnt::Vector const &force) noexcept(noexcept(invMass > 0.f))
{
    acceleration += (force * invMass);
}

tnt::RigidBody::RigidBody(float const &mass, Rectangle const &collision_box)
    : physics{add<PhysicsComponent>(mass, collision_box)} {}

tnt::RigidBody::~RigidBody() noexcept
{
    if (physics != nullptr)
    {
        delete physics;
        physics = nullptr;
    }
}

tnt::PhysicsComponent *tnt::RigidBody::getPhysics() const noexcept { return physics; }

// void tnt::RigidBody::Update()
// {
// velocity += acceleration;
// pos += velocity;
// }