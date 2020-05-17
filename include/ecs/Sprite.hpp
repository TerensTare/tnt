#ifndef TNT_SPRITE_HPP
#define TNT_SPRITE_HPP

#include <map>

#include "ecs/Component.hpp"
#include "ecs/Object.hpp"

// TODO: setIdleAnimation ??

// TODO(maybe):
// addAnimation ??
// move this to the gui folder ??

namespace tnt
{
class Window;
class Timer;

/// @brief A drawable Object.
class Sprite : virtual public Object
{
public:
    /// @brief Create a new @em Sprite to be drawn at @em win.
    /// @param win The @em Window where the @em Sprite should be drawn.
    /// @param filename The file from where the @em Sprite data should be loaded.
    /// @param angle The angle the @em Sprite should be drawn on @em win.
    Sprite(Window const *win, std::string_view filename, float angle = 0.f) noexcept;

    /// @overload This constructor has the same behaviour as the other
    /// constructor, except that it needs an extra @em tnt::Rectangle as a parameter
    /// used to get only a part of the image to the @em Sprite.
    Sprite(Window const *win, std::string_view filename, Rectangle const &area,
           float angle = 0.f) noexcept;

    /// @brief The destructor of the @em Sprite.
    virtual ~Sprite() noexcept;

    /// @brief Get the @em SpriteComponent of the @em Sprite.
    /// @return SpriteComponent*
    SpriteComponent *getSprite() const noexcept;

protected: // (maybe) make these private ??
    SpriteComponent *sprite;
};
} // namespace tnt

#endif //! TNT_SPRITE_HPP