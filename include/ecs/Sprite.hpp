#ifndef TNT_SPRITE_HPP
#define TNT_SPRITE_HPP

#include "ecs/Object.hpp"
#include "math/Rectangle.hpp"

// TODO: setIdleAnimation ??

// TODO(maybe):
// addAnimation ??

typedef struct SDL_Texture SDL_Texture;

namespace tnt
{
    class Window;

    /// @brief A Component that handles drawing to a certain @c tnt::Window.
    class SpriteComponent final : public Component
    {
    public:
        /// @brief Creates a new sprite @c Component from the given @c file to be drawn on @c win.
        /// @param win The target @c tnt::Window where the @c Component should be drawn.
        /// @param file The file from where to load this @c Component's texture.
        SpriteComponent(Window const *win, std::string_view file);

        /// @overload This constructor has the same behaviour as the other constructor,
        /// except that it takes an extra @c tnt::Rectangle that marks the area
        /// of the file which you want to be rendered on @c win.
        /// @note Any SpriteComponent created using this constructor or that changes
        /// it's @c texture by using void setTexture's overload(with a tnt::Rectangle at the end)
        /// is considered clipped.
        SpriteComponent(Window const *win, std::string_view file, Rectangle const &location);

        /// @brief The destructor of the @c Component.
        ~SpriteComponent() noexcept;

        /// @brief Draws this @c Component on @c win at the area covered by @c dest.
        /// @sa void tnt::Window::Draw().
        /// @note This function is just a shorthand to @c tnt::Window::Draw().
        void Draw(Window const *win, Rectangle const &dest, float angle = 0.f) noexcept; // TODO: do you need this ??

        /// @brief Gets the texture that this @c Component holds.
        /// @return SDL_Texture*
        SDL_Texture *getTexture() const noexcept;

        /// @brief Changes the @c texture of this @c Component with
        /// another texture loaded in place.
        /// @param win The target @c tnt::Window where the @c Component should be drawn.
        /// @param filename The file from where to load this @c Component's texture.
        void setTexture(Window const *win, std::string_view filename) noexcept;

        /// @overload This function has the same behaviour as the other @c setTexture
        /// function, except it needs a @c tnt::Rectangle that is used to get
        /// only a part of the image.
        /// @note When this function is called, the @c Component is considered clipped.
        void setTexture(Window const *win, std::string_view filename, Rectangle const &location) noexcept;

        /// @brief Get the width of the @c texture.
        /// @return int
        int getWidth() const noexcept;

        /// @brief Get the height of the @c texture.
        /// @return int
        int getHeight() const noexcept;

    private:
        bool clipped;
        Rectangle clipRect;
        SDL_Texture *texture; // (maybe) this or the AssetManager's maps' values
        // should be weak_ptr's ??

        friend class Animator;
    };

    /// @brief A drawable Object.
    class Sprite : public Object
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
        /// @return tnt::SpriteComponent*
        SpriteComponent *getSprite() const noexcept;

        /// @brief Draw the @c Sprite on @c win at @c dest.
        /// @param win The window where the @c Sprite should be drawed.
        void Draw(Window const *win) noexcept;

    protected: // (maybe) make these private ??
        SpriteComponent *sprite;
    };
} // namespace tnt

#endif //! TNT_SPRITE_HPP