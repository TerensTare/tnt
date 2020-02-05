#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>

#include "Camera.hpp"
#include "gui/Widget.hpp"
#include "Renderable.hpp"

// TODO: destroy EVERY Window member in it's destructor.
// TODO: make THIS a Widget.
namespace tnt
{
class Window : public Widget
{
public:
    Window(std::string_view title, int xpos, int ypos, int width, int height, Uint32 flags, Uint32 renFlags);
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

    std::tuple<int, int, int, int, int> GetBordersSize();

    SDL_Renderer *getRenderer() noexcept;

    void Render();
    void Draw(tnt::Renderable const *obj, const SDL_Rect *srcrect, const tnt::Camera *cam, const double angle = .0, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void Clear();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};
} // namespace tnt

#endif //!WINDOW_HPP