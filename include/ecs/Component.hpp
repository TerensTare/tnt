#ifndef TNT_COMPONENT_HPP
#define TNT_COMPONENT_HPP

#include "math/Vector.hpp"

// TODO:
// Rotate/Scale/Transform: use global coordinates & translate to local coordinates.
// Find a way to handle rendering.
// Find a way to create a Component class/struct that represents all Component types
// (maybe) an Update function.

// TODO(maybe):
// AddForce, Renderable, Widget, Animation, AI ??
// PhysicsComponent::{add/remove}Mass ??
// Rename every component to a shorter name, ex. *Comp ??
// Write a Polygon class so that PhysicsComponent can use it as collision_shape ??
// constructors for Component-s ??
// SpriteComponent should handle a weak_ptr<Window> not a friend class Window
// or get the texture from AssetManager ??

struct SDL_Texture;
typedef struct SDL_Texture SDL_Texture;

typedef struct SDL_Rect
{
    int x;
    int y;
    int w;
    int h;
} SDL_Rect;

typedef struct SDL_FRect
{
    float x;
    float y;
    float w;
    float h;
} SDL_FRect;

namespace tnt
{
class Window;
class Object;

struct infinite_mass : std::exception
{
    const char *what() const noexcept
    {
        return "Object has infinite mass!!";
    }
};

class Component
{
    std::weak_ptr<Object> owner;
};

class RotateComponent : public Component
{
    void setAngle(float &radian) noexcept;
    float getAngle() const noexcept;

    void Rotate(float &radian) noexcept;

protected:
    float angle;
};

class ScaleComponent : public Component
{
    void setScale(Vector const &ratio) noexcept;
    Vector getScale() const noexcept;

    void Scale(Vector const &ratio) noexcept;

protected:
    Vector scale;
};

class PhysicsComponent
{
public:
    void setMass(float &mass);
    float getMass() const noexcept(noexcept(invMass > 0.f));

    Vector getVelocity() const noexcept;
    Vector getAcceleration() const noexcept;

    void applyForce(Vector const &force) noexcept(noexcept(invMass > 0.f));

private:
    float invMass;
    Vector velocity;
    Vector acceleration;
    SDL_FRect collision_box;
};

class SpriteComponent
    : public RotateComponent,
      public ScaleComponent
{
public:
    // TODO: incomplete
    SpriteComponent(std::string_view file);
    ~SpriteComponent() noexcept;

    void Render(Window *target) noexcept;

private:
    bool clipped;
    SDL_Rect clipRect;
    SDL_FRect renderRect;
    SDL_Texture *texture;

    friend class Window;
};
} // namespace tnt

#endif //!TNT_COMPONENT_HPP