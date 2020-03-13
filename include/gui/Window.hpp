#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>

#include "Camera.hpp"
#include "core/Graphics.hpp"
#include "Renderable.hpp"

// TODO: destroy EVERY Window member in it's destructor.
// TODO: make THIS a Widget.
// TODO: multiwindow support.
namespace tnt
{
class Window final
{
public:
    Window(std::string_view title, int xpos, int ypos, int width, int height, Uint32 flags);
    ~Window() noexcept;

    operator SDL_Window *();

    int GetDisplayIndex();

    int SetDisplayMode(const SDL_DisplayMode *mode);
    std::pair<SDL_DisplayMode, int> GetDisplayMode();

    Uint32 GetPixelFormat();
    Uint32 GetID();
    Uint32 GetFlags();

    void SetTitle(char const *title);
    char const *GetTitle();

    void SetIcon(SDL_Surface *icon);

    int *GetBordersSize();

    void Render();
    void Draw(tnt::Renderable const *obj, const SDL_Rect *srcrect, const tnt::Camera *cam, const double angle = .0, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void Clear();

private:
    Graphics &gfx;
    Camera camera;
    SDL_Window *window;
};
} // namespace tnt

#endif //!WINDOW_HPP