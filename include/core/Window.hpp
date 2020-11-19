#ifndef TNT_WINDOW_HPP
#define TNT_WINDOW_HPP

#include <string>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "core/Config.hpp"

// TODO: support drawing fonts.

// TODO(maybe):
// a default window icon ??
// rename to RenderWindow ??

namespace tnt
{
    /// @brief A class representing an application window.
    class TNT_API Window final
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

        /// @overload
        /// @param title The title of the window.
        /// @param width The width of the window.
        /// @param height The height of the window.
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
        int getWidth() const noexcept;

        /// @brief Returns the height of the window.
        /// @return int
        int getHeight() const noexcept;

        /// @brief Change the width of the window.
        /// @param width The new width.
        /// @sa @ref tnt::Window::setHeight
        /// @sa @ref tnt::Window::setSize
        void setWidth(int const width) noexcept;

        /// @brief Change the height of the window.
        /// @param height The new height.
        /// @sa @ref tnt::Window::setWidth
        /// @sa @ref tnt::Window::setSize
        void setHeight(int const height) noexcept;

        /// @brief Change the width and height of the window.
        /// @param width The new width.
        /// @param height The new height.
        /// @sa @ref tnt::Window::setWidth
        /// @sa @ref tnt::Window::setHeight
        void setSize(int const width, int const height) noexcept;

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

        /// @brief Update the screen with rendering performed.
        void Render() noexcept;

        /// @brief Handle some window-related events.
        /// @param events The events handler.
        void handleEvents(SDL_Event const &events) noexcept;

        /// @brief Set the color used for drawing operations (Rect, Line and Clear).
        /// @param color The color used to draw on the Window's renderer.
        void setClearColor(SDL_Color const &color) noexcept;

        /// @overload
        /// @param r Red.
        /// @param g Green.
        /// @param b Blue.
        /// @param a Alpha.
        void setClearColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) noexcept;

        /// @brief Get the color used for drawing operations.
        /// @return SDL_Color
        SDL_Color getClearColor() const noexcept;

        /// @brief Check if the window is open so that events can be handled.
        /// @return bool
        bool isOpened() const noexcept;

    private:
        bool running;
        int w, h;
        SDL_Renderer *renderer;
        SDL_Window *window;
    };

    /// @brief A RAII class which clears the given window on its ctor, and calls Render on its destructor.
    class render_lock final
    {
    public:
        render_lock() noexcept = delete;
        render_lock(render_lock const &) noexcept = delete;
        render_lock &operator=(render_lock const &) noexcept = delete;
        render_lock(render_lock &&) noexcept = delete;
        render_lock &operator=(render_lock &&) noexcept = delete;

        /// @brief Create a new render_lock.
        /// @param win The window to clear/render.
        explicit render_lock(Window const &win) noexcept;

        ~render_lock() noexcept;

    private:
        SDL_Renderer *ren;
    };
} // namespace tnt

#endif //! TNT_WINDOW_HPP