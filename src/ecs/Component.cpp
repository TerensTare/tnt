// This is an independent project of an individual developer. Dear PVS-Studio,
// please check it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com

#include "ecs/Component.hpp"

#include "core/Window.hpp"
#include "fileIO/AssetManager.hpp"
#include "utils/Timer.hpp"

////////////
// rotate //
////////////

tnt::RotateComponent::RotateComponent(float radian) : angle{radian} {}

void tnt::RotateComponent::setAngle(float const& radian) noexcept
{
    angle = radian;
}

float tnt::RotateComponent::getAngle() const noexcept { return angle; }

void tnt::RotateComponent::Rotate(float const& radian) noexcept
{
    angle += radian;
}

///////////
// scale //
///////////

tnt::ScaleComponent::ScaleComponent(tnt::Vector const& ratio) : scale{ratio} {}

tnt::ScaleComponent::ScaleComponent(float x, float y) : scale{x, y} {}

void tnt::ScaleComponent::setScale(Vector const& s) noexcept { scale = s; }
tnt::Vector tnt::ScaleComponent::getScale() const noexcept { return scale; }

void tnt::ScaleComponent::Scale(Vector const& s) noexcept
{
    scale = Vector{scale.x * s.x, scale.y * s.y};
}

/////////////
// physics //
/////////////

tnt::PhysicsComponent::PhysicsComponent(float const& mass,
                                        Rectangle const& collision_box)
    : invMass{1 / mass}, velocity{VECTOR_ZERO}, acceleration{VECTOR_ZERO},
      collisionBox{collision_box}
{}

tnt::PhysicsComponent::PhysicsComponent(float const& mass, float x, float y,
                                        float& w, float& h)
    : invMass{1 / mass}, velocity{VECTOR_ZERO}, acceleration{VECTOR_ZERO},
      collisionBox{x, y, w, h}
{}

void tnt::PhysicsComponent::setMass(float const& mass) { invMass = (1 / mass); }

float tnt::PhysicsComponent::getMass() const noexcept(noexcept(invMass > 0.f))
{
    return (1 / invMass);
}

tnt::Vector tnt::PhysicsComponent::getVelocity() const noexcept
{
    return velocity;
}
tnt::Vector tnt::PhysicsComponent::getAcceleration() const noexcept
{
    return acceleration;
}

tnt::Rectangle tnt::PhysicsComponent::getCollisionBox() const noexcept
{
    return collisionBox;
}

void tnt::PhysicsComponent::applyForce(tnt::Vector const& force) noexcept(
    noexcept(invMass > 0.f))
{
    acceleration += (force * invMass);
}

////////////
// sprite //
////////////

tnt::SpriteComponent::SpriteComponent(Window const* win, std::string_view file)
    : RotateComponent{0.f}, ScaleComponent{VECTOR_ONE}, clipped{false},
      clipRect{0, 0, 0, 0}, texture{AssetManager::This().Image(win, file)}
{
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
}

tnt::SpriteComponent::SpriteComponent(Window const* win, std::string_view file,
                                      Rectangle const& location)
    : RotateComponent{0.f}, ScaleComponent{VECTOR_ONE}, clipped{true},
      clipRect{location}, texture{AssetManager::This().Image(win, file)}
{
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
}

tnt::SpriteComponent::~SpriteComponent() noexcept
{
    SDL_DestroyTexture(&*texture);
    texture = nullptr;
}

void tnt::SpriteComponent::Draw(Window* win, Rectangle& dest) noexcept
{
    win->Draw(this, &static_cast<SDL_Rect>(clipRect),
              &static_cast<SDL_FRect>(dest), static_cast<double>(angle));
}

SDL_Texture* tnt::SpriteComponent::getTexture() const noexcept
{
    return texture;
}

void tnt::SpriteComponent::setTexture(Window const* win,
                                      std::string_view filename) noexcept
{
    texture = AssetManager::This().Image(win, filename);
}

int tnt::SpriteComponent::getWidth() const noexcept { return w; }
int tnt::SpriteComponent::getHeight() const noexcept { return h; }

///////////////
// animation //
///////////////

tnt::AnimationComponent::AnimationComponent(tnt::Window const* win,
                                            std::string_view filename,
                                            int framesNum, float speed,
                                            bool horizontal,
                                            Rectangle const& clip)
    : RotateComponent{0.f}, ScaleComponent{VECTOR_ONE},
      SpriteComponent{win, filename, clip}, startX{clip.x}, startY{clip.y},
      frameCount{framesNum}, animSpeed{speed}, animTime{0},
      timePerFrame{animSpeed / static_cast<float>(frameCount)},
      vertical{!horizontal}, done{false}, loop{true}
{}

void tnt::AnimationComponent::setLoop(bool loop_) noexcept { loop = loop_; }
bool tnt::AnimationComponent::isLoop() const noexcept { return loop; }

bool tnt::AnimationComponent::running() const noexcept { return !done; }

void tnt::AnimationComponent::update(tnt::Timer* timer) noexcept
{
    if (!done)
    {
        animTime += timer->deltaTime().count();
        if (animTime >= animSpeed)
        {
            if (loop)
                animTime -= animSpeed;
            else
            {
                done     = true;
                animTime = animSpeed - timePerFrame;
            }
        }

        if (!vertical)
            clipRect.x = startX + static_cast<int>(animTime / timePerFrame) * w;
        else
            clipRect.y = startY + static_cast<int>(animTime / timePerFrame) * h;
    }
}