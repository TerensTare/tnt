// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <utility>
#include "gui/Window.hpp"

tnt::Window::Window(std::string_view title,
                    int xpos, int ypos, int width, int height,
                    Uint32 flags)
    : gfx{Graphics::This()}, camera{0, 0, width, height},
      window{SDL_CreateWindow(title.data(), xpos, ypos, width, height, flags)} {}

tnt::Window::~Window() noexcept
{
    SDL_DestroyWindow(window);
    gfx.~Graphics();
}

tnt::Window::operator SDL_Window *()
{
    return window;
}

int tnt::Window::GetDisplayIndex()
{
    return SDL_GetWindowDisplayIndex(window);
}

int tnt::Window::SetDisplayMode(const SDL_DisplayMode *mode)
{
    return SDL_SetWindowDisplayMode(window, mode);
}

std::pair<SDL_DisplayMode, int> tnt::Window::GetDisplayMode()
{
    SDL_DisplayMode mode;
    int result{SDL_GetWindowDisplayMode(window, &mode)};
    return std::make_pair(mode, result);
}

Uint32 tnt::Window::GetPixelFormat()
{
    return SDL_GetWindowPixelFormat(window);
}

Uint32 tnt::Window::GetID()
{
    return SDL_GetWindowID(window);
}

Uint32 tnt::Window::GetFlags()
{
    return SDL_GetWindowFlags(window);
}

void tnt::Window::SetTitle(const char *title)
{
    SDL_SetWindowTitle(window, title);
}

char const *tnt::Window::GetTitle()
{
    return SDL_GetWindowTitle(window);
}

void tnt::Window::SetIcon(SDL_Surface *icon)
{
    SDL_SetWindowIcon(window, icon);
}

int *tnt::Window::GetBordersSize()
{
    // guess this should be static, as it is always modified NOT depending to itself.
    static int top, left, bottom, right, result;
    result = SDL_GetWindowBordersSize(window, &top, &left, &bottom, &right);
    int arr[5]{top, left, bottom, right, result};
    return arr;
}
