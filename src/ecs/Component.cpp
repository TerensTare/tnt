// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs/Component.hpp"

#include "core/Window.hpp"
#include "fileIO/AssetManager.hpp"
#include "utils/Timer.hpp"

/////////////
// physics //
/////////////

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

////////////
// sprite //
////////////

tnt::SpriteComponent::SpriteComponent(Window const *win, std::string_view file)
    : clipped{false}, texture{AssetManager::This().Image(win, file)},
      clipRect{0, 0, 0, 0}
{
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
}

tnt::SpriteComponent::SpriteComponent(
    Window const *win, std::string_view file,
    Rectangle const &location)
    : clipped{true}, clipRect{location},
      texture{AssetManager::This().Image(win, file)} {}

tnt::SpriteComponent::~SpriteComponent() noexcept
{
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void tnt::SpriteComponent::Draw(Window const *win, Rectangle const &dest, float angle) noexcept
{
    SDL_RenderCopyEx(
        win->getRenderer(), texture,
        &static_cast<SDL_Rect>(clipRect), &static_cast<SDL_Rect>(dest),
        angle, nullptr, SDL_FLIP_NONE);
}

SDL_Texture *tnt::SpriteComponent::getTexture() const noexcept
{
    return texture;
}

void tnt::SpriteComponent::setTexture(Window const *win, std::string_view filename) noexcept
{
    if (clipped)
        clipped = false;
    SDL_DestroyTexture(texture);
    texture = AssetManager::This().Image(win, filename);
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
}

void tnt::SpriteComponent::setTexture(
    Window const *win, std::string_view filename,
    tnt::Rectangle const &location) noexcept
{
    if (!clipped)
        clipped = true;
    SDL_DestroyTexture(texture);
    texture = AssetManager::This().Image(win, filename);
    clipRect = location;
}

int tnt::SpriteComponent::getWidth() const noexcept
{
    if (clipped)
        return clipRect.w;
    return w;
}
int tnt::SpriteComponent::getHeight() const noexcept
{
    if (clipped)
        return clipRect.h;
    return w;
}

///////////////
// animation //
///////////////

tnt::AnimationComponent::AnimationComponent(
    std::string_view filename, int framesNum, float speed, bool horizontal,
    Rectangle const &clip)
    : startX{clip.x}, startY{clip.y}, frameCount{framesNum}, animSpeed{speed}, animTime{0},
      timePerFrame{animSpeed / static_cast<float>(frameCount)},
      clipRect{clip}, vertical{!horizontal}, done{false}, loop{true} {}

void tnt::AnimationComponent::setLoop(bool loop_) noexcept { loop = loop_; }
bool tnt::AnimationComponent::isLoop() const noexcept { return loop; }

bool tnt::AnimationComponent::running() const noexcept { return !done; }

void tnt::AnimationComponent::update(tnt::Timer *timer) noexcept
{
    if (!done)
    {
        animTime += timer->deltaTime().count();
        if (animTime >= animSpeed)
        {
            if (loop)
                animTime -= static_cast<long long>(animSpeed);
            else
            {
                done = true;
                animTime = static_cast<long long>(animSpeed - timePerFrame);
            }
        }

        if (!vertical)
            clipRect.x = startX + static_cast<int>(animTime / timePerFrame) * clipRect.w;
        else
            clipRect.y = startY + static_cast<int>(animTime / timePerFrame) * clipRect.h;
    }
}