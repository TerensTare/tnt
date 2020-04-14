// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs/RigidBody.hpp"

tnt::RigidBody::RigidBody(float mass, Rectangle const& collision_box)
    : physics { new PhysicsComponent { mass, collision_box } }
{
}

tnt::RigidBody::~RigidBody() noexcept
{
    delete physics;
    physics = nullptr;
}

tnt::PhysicsComponent* tnt::RigidBody::getPhysics() const noexcept { return physics; }

// void tnt::RigidBody::Update()
// {
// velocity += acceleration;
// pos += velocity;
// }