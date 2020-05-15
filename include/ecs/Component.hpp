#ifndef TNT_COMPONENT_HPP
#define TNT_COMPONENT_HPP

#include <string_view>
#include "math/Rectangle.hpp"

// TODO:
// AnimationComponent creates a new SDL_Texture on it's constructor. Fix that!!
// Rotate/Scale/Transform: use global coordinates & translate to local
// coordinates. Removable

// TODO(maybe):
// use a weak_ptr on SpriteComponent ??
// Derive new object from class Object and current Components. Something similar to a policy class ??
// rename AnimationComponent to just Animation ??
// Widget, Animation, AI ??
// PhysicsComponent::{add/remove}Mass ?? also modify collision_box on runtime ??
// Rename every component to a shorter name, ex. *Comp ??
// Write a Polygon class so that PhysicsComponent can use it as collision_shape ??
// SpriteComponent should handle a weak_ptr<Window> not a friend class Window
// or get the texture from AssetManager ??
// also SpriteComponent::Draw(Rectangle const &location) ??
// remove all getters/setters and use Components like C-style structures or POD./ ??
// Serializable<T>/concept ??

typedef struct SDL_Texture SDL_Texture;

namespace tnt
{
  class Window;
  class Timer;

  /// @brief An exception to be thrown when a @em tnt::PhysicsComponent has @em invMass equal to 0.
  /// @note Even thought this exception exists, it is never thrown.
  struct infinite_mass : std::exception
  {
    /// @brief The description of the exception.
    /// @return const char*
    const char *what() const noexcept { return "Object has infinite mass!!"; }
  };

  /// @brief The base class for all the Component types.
  class Component
  {
  };

  class PhysicsComponent final : public Component
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

  /// @brief A Component that handles drawing to a certain @em tnt::Window.
  class SpriteComponent final : public Component
  {
  public:
    /// @brief Creates a new sprite @em Component from the given @em file to be drawn on @em win.
    /// @param win The target @em tnt::Window where the @em Component should be drawn.
    /// @param file The file from where to load this @em Component's texture.
    SpriteComponent(Window const *win, std::string_view file);

    /// @overload This constructor has the same behaviour as the other constructor,
    /// except that it takes an extra @em tnt::Rectangle that marks the area
    /// of the file which you want to be rendered on @em win.
    /// @note Any SpriteComponent created using this constructor or that changes
    /// it's @em texture by using void setTexture's overload(with a tnt::Rectangle at the end)
    /// is considered clipped.
    SpriteComponent(Window const *win, std::string_view file, Rectangle const &location);

    /// @brief The destructor of the @em Component.
    ~SpriteComponent() noexcept;

    /// @brief Draws this @em Component on @em win at the area covered by @em dest.
    /// @sa void tnt::Window::Draw().
    /// @note This function is just a shorthand to @em tnt::Window::Draw().
    void Draw(Window *win, Rectangle &dest, float angle = 0.f) noexcept; // TODO: do you need this ??

    /// @brief Gets the texture that this @em Component holds.
    /// @return SDL_Texture*
    SDL_Texture *getTexture() const noexcept;

    /// @brief Changes the @em texture of this @em Component with
    /// another texture loaded in place.
    /// @param win The target @em tnt::Window where the @em Component should be drawn.
    /// @param filename The file from where to load this @em Component's texture.
    void setTexture(Window const *win, std::string_view filename) noexcept;

    /// @overload This function has the same behaviour as the other @em setTexture
    /// function, except it needs a @em tnt::Rectangle that is used to get
    /// only a part of the image.
    /// @note When this function is called, the @em Component is considered clipped.
    void setTexture(Window const *win, std::string_view filename, Rectangle const &location) noexcept;

    /// @brief Get the width of the @em texture.
    /// @return int
    int getWidth() const noexcept;

    /// @brief Get the height of the @em texture.
    /// @return int
    int getHeight() const noexcept;

  protected:
    bool clipped;
    Rectangle clipRect;
    int w, h;
    SDL_Texture *texture; // maybe this or the AssetManager's maps values
                          // should be weak_ptr's.
  };

  class AnimationComponent final : public Component
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

  template <typename T>
  concept component = std::is_base_of_v<Component, T>;
} // namespace tnt

#endif //! TNT_COMPONENT_HPP