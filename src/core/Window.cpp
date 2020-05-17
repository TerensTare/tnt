// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "core/Window.hpp"
#include "core/Graphics.hpp"
#include "ecs/Sprite.hpp"
#include "utils/Logger.hpp"

#include <SDL2/SDL_image.h>
#include <utility>

tnt::Window::Window(
    std::string_view title,
    int xpos, int ypos, int width, int height,
    Uint32 flags) noexcept
    : running{detail::gfx::Init()}
{
    window = SDL_CreateWindow(title.data(), xpos, ypos, width, height, flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
// SDL_RenderSetLogicalSize(renderer, width, height); // with this, the objects will be resized on window size change.

tnt::Window::Window(std::string_view title, int width, int height) noexcept
    : running{detail::gfx::Init()}
{
    window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

tnt::Window::~Window() noexcept
{
    running = false;

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

int tnt::Window::getDisplayIndex() const noexcept
{
    return SDL_GetWindowDisplayIndex(window);
}

int tnt::Window::setDisplayMode(const SDL_DisplayMode *mode) noexcept
{
    return SDL_SetWindowDisplayMode(window, mode);
}

SDL_DisplayMode *tnt::Window::getDisplayMode() const noexcept
{
    static SDL_DisplayMode mode;
    int result{SDL_GetWindowDisplayMode(window, &mode)};
    if (result != -1)
        return &mode;
    return nullptr;
}

int tnt::Window::getWidth() noexcept
{
    int w;
    SDL_GetWindowSize(window, &w, nullptr);
    return w;
}

int tnt::Window::getHeight() noexcept
{
    int h;
    SDL_GetWindowSize(window, nullptr, &h);
    return h;
}

Uint32 tnt::Window::getPixelFormat() const noexcept
{
    return SDL_GetWindowPixelFormat(window);
}

Uint32 tnt::Window::getID() const noexcept
{
    return SDL_GetWindowID(window);
}

Uint32 tnt::Window::getFlags() const noexcept
{
    return SDL_GetWindowFlags(window);
}

void tnt::Window::setTitle(const char *title) noexcept
{
    SDL_SetWindowTitle(window, title);
}

char const *tnt::Window::getTitle() const noexcept
{
    return SDL_GetWindowTitle(window);
}

void tnt::Window::setIcon(SDL_Surface *icon) noexcept
{
    SDL_SetWindowIcon(window, icon);
}

int *tnt::Window::getBordersSize() const noexcept
{
    static int arr[5];
    arr[4] = SDL_GetWindowBordersSize(window, &arr[0], &arr[1], &arr[2], &arr[3]);
    return arr;
}

void tnt::Window::Clear() noexcept { SDL_RenderClear(renderer); }
void tnt::Window::Render() noexcept { SDL_RenderPresent(renderer); }

// void tnt::Window::Draw(tnt::drawable auto const *obj, SDL_Rect const &srcrect, tnt::camera auto const &cam, SDL_RendererFlip)
// {
//     SDL_RenderCopyExF(renderer, obj->getSprite()->getTexture(), &srcrect, &static_cast<SDL_FRect>(cam.Bounds()), obj->getAngle(), nullptr, flip);
// }

void tnt::Window::setClearColor(SDL_Color const &color) noexcept
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

SDL_Color tnt::Window::getClearColor() const noexcept
{
    SDL_Color col;
    SDL_GetRenderDrawColor(renderer, &col.r, &col.g, &col.b, &col.a);
    return col;
}

void tnt::Window::handleEvents(const SDL_Event &events) noexcept
{
    if (events.type == SDL_QUIT)
        running = false;
}

bool tnt::Window::isOpened() const noexcept { return running; }