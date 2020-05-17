#ifndef TNT_WINDOW_HPP
#define TNT_WINDOW_HPP

#include <string>

#include <SDL2/SDL.h>

#include "ecs/Object.hpp"
#include "core/Camera.hpp"

// TODO: add another simpler constructor which omits some parameters like x, y and (maybe) flags.
// TODO: destroy EVERY Window member in it's destructor.
// TODO: rename to RenderWindow ?? and use as reference/value ??
// TODO: render a Scene when Draw is called.
// TODO: support drawing fonts.
// TODO: rendering in layers (bg1, bg2, bg3,..., player, enemies, etc)
// which (maybe) are stored in a Scene.
// TODO: handle window resizing and some other window-related events.

// TODO(maybe):
// a default window icon ??
// make THIS a Widget ??
namespace tnt
{

    /// @brief A class representing an application window.
    class Window final
    {
    public:
        /// @brief Creates a window with the given parameters.
        /// @param title The title of the window.
        /// @param xpos The x position of the window.
        /// @param ypos The y position of the window.
        /// @param width The width of the window.
        /// @param height The height of the window.
        /// @param flags The flags the window should have.
        Window(std::string_view title, int xpos, int ypos, int width,
               int height, Uint32 flags) noexcept;

        /// @overload This constructor does the same as the other constructor,
        /// except that it draws the window on the center of the screen and
        /// the window is only resizable.
        Window(std::string_view title, int width, int height) noexcept;

        /// @brief Destroys the window and clears the memory occupied by it.
        ~Window() noexcept;

        /// @brief Returns the SDL_Window* of the Window.
        /// @return SDL_Window*
        /// @note SDL_Renderer* can be obtained by calling getRenderer().
        explicit operator SDL_Window *() noexcept;

        /// @brief Returns the SDL_Renderer* of the Window.
        /// @return SDL_Renderer*
        SDL_Renderer *getRenderer() const noexcept;

        /// @brief Get the display index of the window.
        /// The function returns the display index of the display containing the center of the
        /// window, or -1 on error.
        /// @return int
        int getDisplayIndex() const noexcept;

        /// @brief Set the display mode used when a fullscreen window is visible.
        /// By default the window's dimensions and the desktop format
        /// and refresh rate are used. The functions returns 0 on success,
        /// or -1 if setting the display mode failed.
        /// @param mode The mode to use, or NULL for the default mode.
        /// @return int
        int setDisplayMode(const SDL_DisplayMode *mode) noexcept;

        /// @brief Fill in information about the display mode used when a fullscreen
        /// window is visible. Returns nullptr on error.
        /// @return SDL_DisplayMode*
        SDL_DisplayMode *getDisplayMode() const noexcept;

        /// @brief Returns the width of the window.
        /// @return int
        int getWidth() noexcept;

        /// @brief Returns the height of the window.
        /// @return int
        int getHeight() noexcept;

        /// @brief Get the pixel format associated with the window.
        /// @return unsigned int
        Uint32 getPixelFormat() const noexcept;

        /// @brief Get the numeric ID of a window, for logging purposes.
        /// @return unsigned int
        Uint32 getID() const noexcept;

        /// @brief Get the window flags.
        /// @return unsigned int
        Uint32 getFlags() const noexcept;

        /// @brief Set the title of a window, in UTF-8 format.
        /// @param title The desired title for the window.
        void setTitle(char const *title) noexcept;

        /// @brief Get the title of the window.
        /// @return const char*
        char const *getTitle() const noexcept;

        /// @brief Set a surface as the window icon.
        /// @param icon The target SDL_Surface* to be set as an icon.
        void setIcon(SDL_Surface *icon) noexcept;

        /// @brief Get the size of a window's borders (decorations)
        /// around the client area. The function's 5-th return is 0 on success,
        /// @return int* to an array with 5 members.
        /// @note if this function fails (returns -1), the size values of the other array members
        /// will be initialized to 0, 0, 0, 0 (if a non-NULL pointer is provided), as
        /// if the window in question was borderless.
        int *getBordersSize() const noexcept;

        /// @brief Clear the current rendering target with the drawing color.
        /// This function clears the entire rendering target, ignoring the viewport and
        /// the clip rectangle.
        void Clear() noexcept;

        /// @brief Copy a portion of the source Object to the window.
        /// @param obj The source @em drawable @em Object.
        /// @param srcrect The source rectangle.
        /// @param cam The destination camera.
        /// @param flip An SDL_RendererFlip value stating which flipping actions should be performed on the obj's texture.
        /// @note The Object should be @em drawable, meaning it should have a @em SpriteComponent.
        template <drawable D, camera C>
        inline void Draw(D const *obj, SDL_Rect const &srcrect,
                         C const &cam, SDL_RendererFlip flip = SDL_FLIP_NONE) noexcept
        {
            SDL_RenderCopyExF(renderer, obj->getSprite()->getTexture(),
                              &srcrect, &static_cast<SDL_FRect>(cam.Bounds()),
                              obj->getAngle(), nullptr, flip);
        }

        /// @brief Update the screen with rendering performed.
        void Render() noexcept;

        /// @brief Handle some window-related events.
        /// @param events The events handler.
        void handleEvents(SDL_Event const &events) noexcept;

        /// @brief Set the color used for drawing operations (Rect, Line and Clear).
        /// @param color The color used to draw on the Window's renderer.
        void setClearColor(SDL_Color const &color) noexcept;

        /// @brief Get the color used for drawing operations.
        /// @return SDL_Color
        SDL_Color getClearColor() const noexcept;

        /// @brief Check if the window is open so that events can be handled.
        /// @return bool
        bool isOpened() const noexcept;

    private:
        bool running;
        SDL_Renderer *renderer;
        SDL_Window *window;
    };
} // namespace tnt

#endif //! TNT_WINDOW_HPP