#ifndef TNT_WINDOW_HPP
#define TNT_WINDOW_HPP

#include <string>
#include "Renderable.hpp"

// TODO: destroy EVERY Window member in it's destructor.
// TODO: make THIS a Widget.
// TODO: multiwindow support.
// TODO: GetBordersSize returns a local variable. Fix that warning.
// TODO: rename to RenderWindow ?? and use as reference/value ??
// TODO: render a Scene when Draw is called.
// TODO: support drawing fonts.
// TODO: rendering in layers (bg1, bg2, bg3,..., player, enemies, etc)
namespace tnt
{
class SpriteComponent;

class Window final
{
public:
    Window(std::string_view title, int xpos, int ypos, int width, int height, Uint32 flags);
    ~Window() noexcept;

    operator SDL_Window *() noexcept;

    SDL_Renderer *GetRenderer() const noexcept;
    SDL_Texture *LoadTexture(std::string_view filename) noexcept;

    int GetDisplayIndex() const noexcept;

    int SetDisplayMode(const SDL_DisplayMode *mode) noexcept;
    std::pair<SDL_DisplayMode, int> GetDisplayMode() const noexcept;

    Uint32 GetPixelFormat() const noexcept;
    Uint32 GetID() const noexcept;
    Uint32 GetFlags() const noexcept;

    void SetTitle(char const *title) noexcept;
    char const *GetTitle() const noexcept;

    void SetIcon(SDL_Surface *icon) noexcept;

    int *GetBordersSize() const noexcept;

    void Render() noexcept;
    void Draw(tnt::SpriteComponent const *obj, SDL_Rect const *srcrect, SDL_FRect const *cam, const double angle = .0, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void Clear() noexcept;

    void SetClearColor(SDL_Color const &color) noexcept;

private:
    SDL_Renderer *renderer;
    SDL_Window *window;
};
} // namespace tnt

#endif //!TNT_WINDOW_HPP