#ifndef TNT_WINDOW_HPP
#define TNT_WINDOW_HPP

#include <SDL2/SDL.h>

#include <string>

// TODO: destroy EVERY Window member in it's destructor.
// TODO: make THIS a Widget.
// TODO: multiwindow support.
// TODO: GetBordersSize returns a local variable. Fix that warning.
// TODO: rename to RenderWindow ?? and use as reference/value ??
// TODO: render a Scene when Draw is called.
// TODO: support drawing fonts.
// TODO: rendering in layers (bg1, bg2, bg3,..., player, enemies, etc)
// which (maybe) are stored in a Scene.
// TODO: handle window resizing.
// TODO: batch rendering.

// TODO(maybe):
// std::optional return for functions that may have errors.
namespace tnt
{
    class SpriteComponent;
    class Sprite;

    class Window final
    {
      public:
        Window(std::string_view title, int xpos, int ypos, int width,
               int height, Uint32 flags);
        ~Window() noexcept;

        operator SDL_Window *() noexcept;

        SDL_Renderer *getRenderer() const noexcept;

        int getDisplayIndex() const noexcept;

        int setDisplayMode(const SDL_DisplayMode *mode) noexcept;
        std::pair<SDL_DisplayMode, int> getDisplayMode() const noexcept;

        int getWidth() noexcept;
        int getHeight() noexcept;

        Uint32 getPixelFormat() const noexcept;
        Uint32 getID() const noexcept;
        Uint32 getFlags() const noexcept;

        void setTitle(char const *title) noexcept;
        char const *getTitle() const noexcept;

        void setIcon(SDL_Surface *icon) noexcept;

        int *getBordersSize() const noexcept;

        void Render() noexcept;
        void Draw(tnt::SpriteComponent const *obj, SDL_Rect const *srcrect,
                  SDL_FRect const *cam, const double angle = .0,
                  SDL_RendererFlip flip = SDL_FLIP_NONE);
        void Draw(tnt::Sprite const *obj, SDL_Rect const *srcrect,
                  SDL_FRect const *cam, const double angle = .0,
                  SDL_RendererFlip flip = SDL_FLIP_NONE);
        void Clear() noexcept;

        void setClearColor(SDL_Color const &color) noexcept;
        SDL_Color getClearColor() const noexcept;

      private:
        SDL_Renderer *renderer;
        SDL_Window *window;
    };
} // namespace tnt

#endif //! TNT_WINDOW_HPP