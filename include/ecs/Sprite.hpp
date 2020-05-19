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
        /// @brief Create a new @c Sprite to be drawn at @c win.
        /// @param win The @c Window where the @c Sprite should be drawn.
        /// @param filename The file from where the @c Sprite data should be loaded.
        /// @param angle The angle the @c Sprite should be drawn on @c win.
        Sprite(Window const *win, std::string_view filename, float angle = 0.f) noexcept;

        /// @overload This constructor has the same behaviour as the other
        /// constructor, except that it needs an extra @c tnt::Rectangle as a parameter
        /// used to get only a part of the image to the @c Sprite.
        Sprite(Window const *win, std::string_view filename, Rectangle const &area,
               float angle = 0.f) noexcept;

        /// @brief The destructor of the @c Sprite.
        virtual ~Sprite() noexcept;

        /// @brief Get the @c SpriteComponent of the @c Sprite.
        /// @return SpriteComponent*
        SpriteComponent *getSprite() const noexcept;

    protected: // (maybe) make these private ??
        SpriteComponent *sprite;
    };
} // namespace tnt

#endif //! TNT_SPRITE_HPP