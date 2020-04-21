#ifndef TNT_COMPONENT_HPP
#define TNT_COMPONENT_HPP

#include <SDL2/SDL.h>
#include <string_view>

#include "math/Rectangle.hpp"

// TODO:
// AnimationComponent creates a new SDL_Texture on it's constructor. Fix that!!
// (maybe) use a weak_ptr on SpriteComponent ??
// Rotate/Scale/Transform: use global coordinates & translate to local
// coordinates. Removable

// TODO(maybe):
// rename AnimationComponent to just Animation ??
// Widget, Animation, AI ??
// PhysicsComponent::{add/remove}Mass ?? also modify collision_box on runtime ??
// Rename every component to a shorter name, ex. *Comp ??
// Write a Polygon class so that PhysicsComponent can use it as collision_shape
// ?? SpriteComponent should handle a weak_ptr<Window> not a friend class Window
// or get the texture from AssetManager ??
// also SpriteComponent::Draw(Rectangle const &location) ??
// remove all getters/setters and use Components like C-style structures or POD.
// ?? Serializable<T>/concept ??

namespace tnt
{
class Window;
class Timer;

struct infinite_mass : std::exception
{
  const char *what() const noexcept { return "Object has infinite mass!!"; }
};

class Component
{
  // public:
  //   Component(char const *name_) : name{name_} {}
  //   char const *getName() const noexcept;

  // protected:
  //   char const *name; // a string that identifies each class.
};

class RotateComponent : public Component
{
public:
  explicit RotateComponent(float radian);

  void setAngle(float const &radian) noexcept;
  float getAngle() const noexcept;

  void Rotate(float const &radian) noexcept;

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

class PhysicsComponent : virtual public Component
{
public:
  PhysicsComponent(float const &mass, Rectangle const &collision_box);
  PhysicsComponent(float const &mass, float x, float y, float &w, float &h);

  void setMass(float const &mass);
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

  void Draw(Window *win,
            Rectangle &dest) noexcept; // TODO: do you need this ??

  SDL_Texture *getTexture() const noexcept;
  void setTexture(Window const *win, std::string_view filename) noexcept;

  int getWidth() const noexcept;
  int getHeight() const noexcept;

protected:
  bool clipped;
  Rectangle clipRect;
  int w, h;
  SDL_Texture *texture; // maybe this or the AssetManager's maps values
                        // should be weak_ptr's.
};

class AnimationComponent
    : virtual public RotateComponent,
      virtual public ScaleComponent
{
public:
  AnimationComponent(std::string_view filename, int framesNum, float speed,
                     bool horizontal, Rectangle const &clip);

  void setLoop(bool loop_) noexcept;
  bool isLoop() const noexcept;

  bool running() const noexcept;

  // TODO: find a better workaround for this
  void update(Timer *timer) noexcept;

private:
  bool done;
  bool loop;
  bool vertical;
  float animSpeed;
  float timePerFrame;
  float startX, startY;
  int frameCount;
  long long animTime;
  Rectangle clipRect;
};
} // namespace tnt

#endif //! TNT_COMPONENT_HPP