// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs/Component.hpp"
#include "core/Window.hpp"

////////////
// rotate //
////////////

void tnt::RotateComponent::setAngle(float &radian) noexcept { angle = radian; }
float tnt::RotateComponent::getAngle() const noexcept { return angle; }

void tnt::RotateComponent::Rotate(float &radian) noexcept { angle += radian; }

bool tnt::RotateComponent::operator==(tnt::RotateComponent const &rhs) const noexcept
{
    return (angle == rhs.angle);
}

///////////
// scale //
///////////

void tnt::ScaleComponent::setScale(Vector const &s) noexcept { scale = s; }
tnt::Vector tnt::ScaleComponent::getScale() const noexcept { return scale; }

void tnt::ScaleComponent::Scale(Vector const &s) noexcept { scale = std::move(Vector{scale.x * s.x, scale.y = s.y}); }

bool tnt::ScaleComponent::operator==(ScaleComponent const &rhs) const noexcept
{
    return (scale == rhs.scale);
}

/////////////
// physics //
/////////////

void tnt::PhysicsComponent::setMass(float &mass) { invMass = (1 / mass); }
float tnt::PhysicsComponent::getMass() const noexcept(noexcept(invMass > 0.f)) { return 1 / invMass; }

tnt::Vector tnt::PhysicsComponent::getVelocity() const noexcept { return velocity; }
tnt::Vector tnt::PhysicsComponent::getAcceleration() const noexcept { return acceleration; }

void tnt::PhysicsComponent::applyForce(tnt::Vector const &force) noexcept(noexcept(invMass > 0.f))
{
    acceleration += (force * invMass);
}

////////////
// sprite //
////////////

tnt::SpriteComponent::SpriteComponent(std::string_view file)
{
}

tnt::SpriteComponent::~SpriteComponent() noexcept
{
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void tnt::SpriteComponent::Render(tnt::Window *target) noexcept
{
    target->Draw(this, &clipRect, (SDL_FRect const *)renderRect, angle);
}