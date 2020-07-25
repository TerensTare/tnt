// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs/Particle.hpp"
#include "core/Window.hpp"

tnt::Particle::Particle(
    Window const *win, uint8_t r, uint8_t g, uint8_t b, uint8_t a,
    Vector const &speed, float const &radius_, float time)
    : Sprite{win, "assets/particle.png"}, RigidBody{1.f},
      lifetime{time}, radius{radius_}, alive{true},
      bounds{(float)win->getWidth(), (float)win->getHeight()}
{
    SDL_SetTextureColorMod(sprite->getTexture(), r, g, b);
    SDL_SetTextureAlphaMod(sprite->getTexture(), a);
    physics->applyForce(speed); // mass is 1.f, accel is 1.f
}

tnt::Particle::~Particle() noexcept
{
    if (alive)
        alive = false;
    lifetime = 0;
}

void tnt::Particle::Update(long long elapsed) noexcept
{
    if (alive)
    {
        if (lifetime > 0)
            lifetime = lifetime - elapsed;
        if (lifetime == 0 || lifetime < -1 ||
            position.x > bounds.x || position.y > bounds.y)
            alive = false;
        else
            doPhysics(elapsed / 1000);
    }
}

void tnt::Particle::Draw(Window const *win) noexcept
{
    if (alive)
        Draw(win);
}