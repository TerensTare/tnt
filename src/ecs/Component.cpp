// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs/Component.hpp"
#include "core/Window.hpp"
#include "fileIO/AssetManager.hpp"

////////////
// rotate //
////////////

tnt::RotateComponent::RotateComponent(float radian)
    : angle{radian} {}

void tnt::RotateComponent::setAngle(float &radian) noexcept { angle = radian; }
float tnt::RotateComponent::getAngle() const noexcept { return angle; }

void tnt::RotateComponent::Rotate(float &radian) noexcept { angle += radian; }

///////////
// scale //
///////////

tnt::ScaleComponent::ScaleComponent(tnt::Vector const &ratio)
    : scale{ratio} {}

tnt::ScaleComponent::ScaleComponent(float x, float y)
    : scale{x, y} {}

void tnt::ScaleComponent::setScale(Vector const &s) noexcept { scale = s; }
tnt::Vector tnt::ScaleComponent::getScale() const noexcept { return scale; }

void tnt::ScaleComponent::Scale(Vector const &s) noexcept { scale = Vector{scale.x * s.x, scale.y * s.y}; }

/////////////
// physics //
/////////////

tnt::PhysicsComponent::PhysicsComponent(float &mass, SDL_FRect const &collision_box)
    : invMass{1 / mass}, velocity{VECTOR_ZERO},
      acceleration{VECTOR_ZERO}, collisionBox{collision_box} {}

tnt::PhysicsComponent::PhysicsComponent(float &mass, float x, float y, float &w, float &h)
    : invMass{1 / mass}, velocity{VECTOR_ZERO},
      acceleration{VECTOR_ZERO}, collisionBox{x, y, w, h} {}

void tnt::PhysicsComponent::setMass(float &mass)
{
    invMass = (1 / mass);
}

float tnt::PhysicsComponent::getMass() const noexcept(noexcept(invMass > 0.f)) { return 1 / invMass; }

tnt::Vector tnt::PhysicsComponent::getVelocity() const noexcept { return velocity; }
tnt::Vector tnt::PhysicsComponent::getAcceleration() const noexcept { return acceleration; }

SDL_FRect tnt::PhysicsComponent::getCollisionBox() const noexcept { return collisionBox; }

void tnt::PhysicsComponent::applyForce(tnt::Vector const &force) noexcept(noexcept(invMass > 0.f))
{
    acceleration += (force * invMass);
}

////////////
// sprite //
////////////

tnt::SpriteComponent::SpriteComponent(
    Window const *win, std::string_view file)
    : RotateComponent{0.f}, ScaleComponent{VECTOR_ONE},
      clipped{false}, clipRect{0, 0, 0, 0},
      texture{AssetManager::This().Image(win, file)} {}

tnt::SpriteComponent::SpriteComponent(
    Window const *win, std::string_view file,
    SDL_Rect const &location)
    : RotateComponent{0.f}, ScaleComponent{VECTOR_ONE},
      clipped{true}, clipRect{location},
      texture{AssetManager::This().Image(win, file)} {}

tnt::SpriteComponent::~SpriteComponent() noexcept
{
    SDL_DestroyTexture(&*texture);
    texture = nullptr;
}

void tnt::SpriteComponent::Draw(tnt::Window *target, SDL_FRect const& dest) noexcept
{
    target->Draw(this, &clipRect, &dest);
}

SDL_Texture *tnt::SpriteComponent::getTexture() const noexcept { return texture; }

void tnt::SpriteComponent::setTexture(Window const *win, std::string_view filename) noexcept
{
    texture = AssetManager::This().Image(win, filename);
}