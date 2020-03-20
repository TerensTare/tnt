#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>

#include "Camera.hpp"
#include "Renderable.hpp"
#include "fileIO/AssetManager.hpp"

// TODO: destroy EVERY Window member in it's destructor.
// TODO: make THIS a Widget.
// TODO: multiwindow support.
// TODO: GetBordersSize returns a local variable. Fix that warning.
// TODO: rename to RenderWindow ?? and use as reference/value ??
namespace tnt
{
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
    void Draw(tnt::Renderable const *obj, const SDL_Rect &srcrect, const tnt::Camera &cam, const double angle = .0, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void Clear() noexcept;

    void SetClearColor(SDL_Color const &color) noexcept;

private:
    Camera camera;
    SDL_Renderer *renderer;
    SDL_Window *window;
    std::shared_ptr<AssetManager> assets;
};
} // namespace tnt

#endif //!WINDOW_HPP