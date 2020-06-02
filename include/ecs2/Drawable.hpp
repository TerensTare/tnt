#ifndef TNT_ECS2_DRAWABLE_HPP
#define TNT_ECS2_DRAWABLE_HPP

#include "ecs2/Object.hpp"
#include "ecs2/Transformable.hpp"
#include "math/Rectangle.hpp"

typedef struct SDL_Texture SDL_Texture;

namespace tnt
{
    class Window;
}

// TODO:
// SpriteComp::getWidth/getHeight should be multiplied with the scale.x/scale.y

namespace tnt::ecs2
{

    /// @brief A Component that handles drawing to a certain @c tnt::Window.
    class SpriteComp : public ecs2::Component
    {
    public:
        /// @brief Creates a new sprite @c Component from the given @c file to be drawn on @c win.
        /// @param win The target @c tnt::Window where the @c Component should be drawn.
        /// @param file The file from where to load this @c Component's texture.
        SpriteComp(Window const *win, std::string_view file);

        /// @overload This constructor has the same behaviour as the other constructor,
        /// except that it takes an extra @c tnt::Rectangle that marks the area
        /// of the file which you want to be rendered on @c win.
        /// @note Any SpriteComponent created using this constructor or that changes
        /// it's @c texture by using void setTexture's overload(with a tnt::Rectangle at the end)
        /// is considered clipped.
        SpriteComp(Window const *win, std::string_view file, Rectangle const &location);

        /// @brief The destructor of the @c Component.
        ~SpriteComp() noexcept;

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

    protected:
        bool clipped;
        Rectangle clipRect;
        SDL_Texture *texture; // (maybe) this or the AssetManager's maps' values
        // should be weak_ptr's ??
    };

    class Sprite
        : public ecs2::Object<Sprite, RotateComp, ScaleComp,
                              TransformComp, SpriteComp>
    {
        using Object::Object; // use Object's nice constructor.

    public:
        void Draw(tnt::Window const *win) noexcept;
    };
} // namespace tnt::ecs2

#endif //!TNT_ECS2_DRAWABLE_HPP