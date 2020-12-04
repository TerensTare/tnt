// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "core/Window.hpp"
#include "utils/Logger.hpp"

namespace tnt::detail::gfx
{
    inline static bool init{false};
    inline static short window_count{0};

    inline static bool Init() noexcept
    {
        if (!detail::gfx::init)
        {
            if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            {
                tnt::logger::error("Couldn't initalize SDL2!! Error: {}", SDL_GetError());
                return false;
            }

#define flags (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP)

            if ((IMG_Init(flags) & flags) != flags)
            {
                tnt::logger::error("Couldn't initialize SDL_Image!! Error: {}", IMG_GetError());
                return false;
            }

#undef flags

            if (TTF_Init() == -1)
            {
                tnt::logger::error("Couldn't initalize SDL_ttf!! Error: {}", TTF_GetError());
                return false;
            }

            tnt::logger::debug("Successfully initialized all graphics systems.");
        }

        ++window_count;
        detail::gfx::init = true;
        return true;
    }

    inline static void Quit() noexcept
    {
        if (--window_count == 0)
        {
            TTF_Quit();
            IMG_Quit();
            SDL_Quit();
            init = false;
        }
    }

} // namespace tnt::detail::gfx

tnt::Window::Window(std::string_view title,
                    int xpos, int ypos, int width, int height,
                    Uint32 flags) noexcept
    : running{detail::gfx::Init()}
{
    window = SDL_CreateWindow(title.data(), xpos, ypos, width, height, flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_GetWindowSize(window, &w, &h);
}

tnt::Window::Window(std::string_view title, int width, int height) noexcept
    : running{detail::gfx::Init()}
{
    window = SDL_CreateWindow(
        title.data(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_GetWindowSize(window, &w, &h);
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
    int const &result{SDL_GetWindowDisplayMode(window, &mode)};
    if (result != -1)
        return &mode;
    return nullptr;
}

int tnt::Window::getWidth() const noexcept { return w; }

int tnt::Window::getHeight() const noexcept { return h; }

void tnt::Window::setWidth(int const width) noexcept
{
    w = width;
    SDL_SetWindowSize(window, w, h);
}

void tnt::Window::setHeight(int const height) noexcept
{
    h = height;
    SDL_SetWindowSize(window, w, h);
}

void tnt::Window::setSize(int width, int height) noexcept
{
    w = width;
    h = height;
    SDL_SetWindowSize(window, w, h);
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

void tnt::Window::setIcon(std::string_view icon) noexcept
{
    if (SDL_RWops *io = SDL_RWFromFile(icon.data(), "r"); io != nullptr)
    {
        if (SDL_Surface *surf = IMG_LoadICO_RW(io); surf != nullptr)
        {
            SDL_SetWindowIcon(window, surf);
            SDL_FreeSurface(surf);
        }
        else
        {
            logger::error("tnt::Window::setIcon: Couldn't load icon surface!! {}", SDL_GetError());
            SDL_RWclose(io);
        }

        SDL_RWclose(io);
    }
    else
        logger::error("tnt::Window::setIcon: Couldn't open SDL_RWOps!! {}", SDL_GetError());
}

int *tnt::Window::getBordersSize() const noexcept
{
    static int arr[5];
    arr[4] = SDL_GetWindowBordersSize(window, &arr[0], &arr[1], &arr[2], &arr[3]);
    return arr;
}

void tnt::Window::Clear() noexcept { SDL_RenderClear(renderer); }
void tnt::Window::Render() noexcept { SDL_RenderPresent(renderer); }

void tnt::Window::setClearColor(SDL_Color const &color) noexcept
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void tnt::Window::setClearColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) noexcept
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
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
    if (events.type == SDL_WINDOWEVENT &&
        events.window.event == SDL_WINDOWEVENT_RESIZED)
    {
        w = events.window.data1;
        h = events.window.data2;
    }
}

bool tnt::Window::isOpened() const noexcept { return running; }

tnt::render_lock::render_lock(tnt::Window const &win) noexcept
    : ren{win.getRenderer()}
{
    SDL_RenderClear(ren);
}

tnt::render_lock::~render_lock() noexcept { SDL_RenderPresent(ren); }