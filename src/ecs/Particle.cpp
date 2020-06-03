// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs/Particle.hpp"
#include "core/Window.hpp"

tnt::Particle::Particle(
    Window const *win, std::string_view filename,
    Vector const &speed, float &radius_,
    Rectangle const &area, long long time)
    : Sprite{win, filename}, RigidBody{1.f, area},
      lifetime{time}, rect{area}, radius{radius_}, alive{true} {}

tnt::Particle::~Particle() noexcept
{
    if (alive)
        alive = false;
    lifetime = 0;
    rect = Rectangle{0, 0, 0, 0};
}

void tnt::Particle::Update(long long elapsed) noexcept
{
    if (alive)
    {
        if (lifetime > 0)
            lifetime = lifetime - elapsed;
        if (rect.Outside(position) || lifetime < 0)
            alive = false;
        else
            position = position + (physics->getVelocity() * (elapsed * 1.0 / 1000));
    }
}