#ifndef TNT_COMPONENT_HPP
#define TNT_COMPONENT_HPP

#include <string_view>
#include <SDL2/SDL.h>
#include "math/Rectangle.hpp"

// TODO:
// Rotate/Scale/Transform: use global coordinates & translate to local coordinates.
// Find a way to handle rendering.

// TODO(maybe):
// Widget, Animation, AI ??
// PhysicsComponent::{add/remove}Mass ?? also modify collision_box on runtime ??
// Rename every component to a shorter name, ex. *Comp ??
// Write a Polygon class so that PhysicsComponent can use it as collision_shape ??
// SpriteComponent should handle a weak_ptr<Window> not a friend class Window
// or get the texture from AssetManager ??
// also SpriteComponent::Draw(Rectangle const &location) ??
// remove all getters/setters and use Components like C-style structures or POD. ??

namespace tnt
{
class Window;

struct infinite_mass : std::exception
{
    const char *what() const noexcept
    {
        return "Object has infinite mass!!";
    }
};

class Component
{
};

class RotateComponent : public Component
{
public:
    explicit RotateComponent(float radian);

    void setAngle(float &radian) noexcept;
    float getAngle() const noexcept;

    void Rotate(float &radian) noexcept;

protected:
    float angle;
};

class ScaleComponent : public Component
{
public:
    explicit ScaleComponent(Vector const &ratio);
    ScaleComponent(float x, float y);

    void setScale(Vector const &ratio) noexcept;
    Vector getScale() const noexcept;

    void Scale(Vector const &ratio) noexcept;

protected:
    Vector scale;
};

class PhysicsComponent
{
public:
    PhysicsComponent(float &mass, Rectangle const &collision_box);
    PhysicsComponent(float &mass, float x, float y, float &w, float &h);

    void setMass(float &mass);
    float getMass() const noexcept(noexcept(invMass > 0.f));

    Vector getVelocity() const noexcept;
    Vector getAcceleration() const noexcept;

    Rectangle getCollisionBox() const noexcept;

    void applyForce(Vector const &force) noexcept(noexcept(invMass > 0.f));

private:
    float invMass;
    Vector velocity;
    Vector maxVelocity; // necessary ??
    Vector acceleration;
    Rectangle collisionBox;
};

// TODO: incomplete ctor/class (load texture and set renderRect)
// TODO(maybe): handle font textures ??
// TODO(maybe): get/set renderTarget
class SpriteComponent
    : virtual public RotateComponent,
      virtual public ScaleComponent
{
public:
    SpriteComponent(Window const *win, std::string_view file);
    SpriteComponent(Window const *win, std::string_view file, Rectangle const &location);

    virtual ~SpriteComponent() noexcept;

    void Draw(Window *win, Rectangle &dest) noexcept; // TODO: do you need this ??

    SDL_Texture *getTexture() const noexcept;
    void setTexture(Window const *win, std::string_view filename) noexcept;

private:
    bool clipped;
    Rectangle clipRect;
    int w, h;
    SDL_Texture *texture; // maybe this or the AssetManager's maps values should be weak_ptr's.
};

class AnimationComponent
    : public SpriteComponent
{
protected:
    // TODO: WIP
};
} // namespace tnt

#endif //!TNT_COMPONENT_HPP