// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs/RigidBody.hpp"

tnt::RigidBody::RigidBody(float &mass, SDL_FRect const &collision_box)
{
    addComponent<PhysicsComponent>();
    getComponent<PhysicsComponent>()->setMass(mass);
}

tnt::PhysicsComponent *tnt::RigidBody::getPhysics() const noexcept { return getComponent<PhysicsComponent>(); }

// void tnt::RigidBody::Update()
// {
// velocity += acceleration;
// pos += velocity;
// }