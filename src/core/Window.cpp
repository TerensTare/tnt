// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "core/Graphics.hpp"
#include "core/Window.hpp"
#include "utils/Logger.hpp"
#include "ecs/Sprite.hpp"

#include <utility>
#include <SDL2/SDL_image.h>

tnt::Window::Window(
    std::string_view title,
    int xpos, int ypos, int width, int height,
    Uint32 flags)
{
    detail::gfx::Init();
    // SDL_SetHint(SDL_HINT_RENDER_BATCHING, "0");

    window = SDL_CreateWindow(title.data(), xpos, ypos, width, height, flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

tnt::Window::~Window() noexcept
{
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    detail::gfx::Quit();
}

tnt::Window::operator SDL_Window *() noexcept
{
    return window;
}

SDL_Renderer *tnt::Window::getRenderer() const noexcept
{
    return renderer;
}

SDL_Texture *tnt::Window::LoadTexture(std::string_view filename) noexcept
{
    SDL_Texture *ret{IMG_LoadTexture(renderer, filename.data())};
    if (!ret)
    {
        tnt::logger::debug("Couldn't load texture {}!! Error: {}\n", filename, SDL_GetError());
        return nullptr;
    }
    return ret;
}

int tnt::Window::GetDisplayIndex() const noexcept
{
    return SDL_GetWindowDisplayIndex(window);
}

int tnt::Window::SetDisplayMode(const SDL_DisplayMode *mode) noexcept
{
    return SDL_SetWindowDisplayMode(window, mode);
}

std::pair<SDL_DisplayMode, int> tnt::Window::GetDisplayMode() const noexcept
{
    SDL_DisplayMode mode;
    int result{SDL_GetWindowDisplayMode(window, &mode)};
    return std::make_pair(mode, result);
}

Uint32 tnt::Window::GetPixelFormat() const noexcept
{
    return SDL_GetWindowPixelFormat(window);
}

Uint32 tnt::Window::GetID() const noexcept
{
    return SDL_GetWindowID(window);
}

Uint32 tnt::Window::GetFlags() const noexcept
{
    return SDL_GetWindowFlags(window);
}

void tnt::Window::SetTitle(const char *title) noexcept
{
    SDL_SetWindowTitle(window, title);
}

char const *tnt::Window::GetTitle() const noexcept
{
    return SDL_GetWindowTitle(window);
}

void tnt::Window::SetIcon(SDL_Surface *icon) noexcept
{
    SDL_SetWindowIcon(window, icon);
}

int *tnt::Window::GetBordersSize() const noexcept
{
    int arr[5];
    arr[4] = SDL_GetWindowBordersSize(window, &arr[0], &arr[1], &arr[2], &arr[3]);
    return arr;
}

void tnt::Window::Clear() noexcept { SDL_RenderClear(renderer); }
void tnt::Window::Render() noexcept { SDL_RenderPresent(renderer); }

void tnt::Window::Draw(tnt::SpriteComponent const *obj, SDL_Rect const *srcrect, SDL_FRect const *cam, double angle, SDL_RendererFlip flip)
{
    SDL_RenderCopyExF(renderer, &(*obj->getTexture()), srcrect, cam, angle, NULL, flip);
}

void tnt::Window::Draw(const tnt::Sprite *obj, const SDL_Rect *srcrect, const SDL_FRect *cam, double angle, SDL_RendererFlip flip)
{
    SDL_RenderCopyExF(renderer, &(*obj->getSprite()->getTexture()), srcrect, cam, angle, NULL, flip);
}

void tnt::Window::SetClearColor(SDL_Color const &color) noexcept
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}