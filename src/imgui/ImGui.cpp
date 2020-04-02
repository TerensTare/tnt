// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <SDL2/SDL_events.h>

#include "exp/imgui/ImGui.hpp"
#include "core/InputManager.hpp"
#include "core/Window.hpp"
#include "utils/Logger.hpp"

namespace tnt::ImGui
{
struct global_config
{
    int w;
    int h;
    SDL_Color bg;
} * global_cfg{new global_config{.w = 0, .h = 0, .bg = SDL_Color{10, 210, 255, 255}}};

struct button_config
{
    int w;
    int h;

    SDL_Color idle_color;
    SDL_Color active_color;
} * button_cfg{new button_config{
        .w = 60,
        .h = 40,
        .idle_color = SDL_Color{100, 100, 100, 255},
        .active_color = SDL_Color{20, 110, 200, 255}}};

struct context_t
{
    int mouse_x, mouse_y;
    bool mouse_down;
    int active, hot;
    SDL_Event event;
};

/////////////
// getters //
/////////////

int get_window_w() noexcept { return global_cfg->w; }
int get_window_h() noexcept { return global_cfg->h; }

unsigned char *get_bg_color() noexcept
{
    unsigned char r{global_cfg->bg.r};
    unsigned char g{global_cfg->bg.g};
    unsigned char b{global_cfg->bg.b};
    unsigned char a{global_cfg->bg.a};
    unsigned char arr[4]{r, g, b, a};
    return arr;
}

int get_button_w() noexcept { return button_cfg->w; }
int get_button_h() noexcept { return button_cfg->h; }

unsigned char *get_button_idle_color() noexcept
{
    unsigned char r{button_cfg->idle_color.r};
    unsigned char g{button_cfg->idle_color.g};
    unsigned char b{button_cfg->idle_color.b};
    unsigned char a{button_cfg->idle_color.a};
    unsigned char arr[4]{r, g, b, a};
    return arr;
}

unsigned char *get_button_active_color() noexcept
{
    unsigned char r{button_cfg->active_color.r};
    unsigned char g{button_cfg->active_color.g};
    unsigned char b{button_cfg->active_color.b};
    unsigned char a{button_cfg->active_color.a};
    unsigned char arr[4]{r, g, b, a};
    return arr;
}

/////////////
// setters //
/////////////

void set_button_w(int w) noexcept { button_cfg->w = w; }
void set_button_h(int h) noexcept { button_cfg->h = h; }

void set_button_idle_color(
    unsigned char r, unsigned char g,
    unsigned char b, unsigned char a) noexcept
{
    button_cfg->idle_color = SDL_Color{r, g, b, a};
}

void set_button_active_color(
    unsigned char r, unsigned char g,
    unsigned char b, unsigned char a) noexcept
{
    button_cfg->active_color = SDL_Color{r, g, b, a};
}

/////////////////
// local utils //
/////////////////

auto on_rect = [](context_t *ctx, int x, int y, int w, int h) -> bool {
    return ((ctx->mouse_x >= x) && (ctx->mouse_x < x + w) &&
            (ctx->mouse_y >= y) && (ctx->mouse_y < y + h));
};

///////////////////
// context stuff //
///////////////////

context_t *make_context(int w, int h) noexcept
{
    if (global_cfg->w == 0) // (maybe) make these 0 on destroy_context
        global_cfg->w = w;
    if (global_cfg->h == 0)
        global_cfg->h = h;

    context_t *ctx{new context_t};
    auto pos{InputManager::This().MousePosition()};

    ctx->mouse_x = pos.first;
    ctx->mouse_y = pos.second;
    ctx->mouse_down = InputManager::This().MouseButtonDown(0);
    ctx->active = 0;
    ctx->hot = 0;

    return ctx;
}

void update_context(context_t *ctx) noexcept
{
    auto pos{InputManager::This().MousePosition()};

    ctx->mouse_x = pos.first;
    ctx->mouse_y = pos.second;
    ctx->mouse_down = InputManager::This().MouseButtonDown(0);
    SDL_PollEvent(&ctx->event);
}

void destroy_context(context_t *ctx) noexcept
{
    delete button_cfg;
    button_cfg = nullptr;

    delete global_cfg;
    global_cfg = nullptr;

    ctx->mouse_x = 0;
    ctx->mouse_y = 0;
    ctx->mouse_down = false;
    ctx->active = 0;
    ctx->hot = 0;

    delete ctx;
    ctx = nullptr;
}

/////////////
// widgets //
/////////////

int button(context_t *ctx, Window *win, int id, int x, int y) noexcept
{
    if (on_rect(ctx, x, y, button_cfg->w, button_cfg->h))
    {
        logger::debug("mouse on button");
        ctx->hot = id;
        if (ctx->active == 0 && ctx->mouse_down) // left pressed
            ctx->active = id;
    }

    SDL_Rect dst{x, y, button_cfg->w, button_cfg->h};

    if (ctx->hot == id)
    {
        if (ctx->active == id)
        {
            SDL_RenderDrawRect(win->getRenderer(), &dst);
            win->setClearColor(button_cfg->active_color);
            SDL_RenderFillRect(win->getRenderer(), &dst);
            win->setClearColor(global_cfg->bg);
        }
        else
        {
            SDL_RenderDrawRect(win->getRenderer(), &dst);
            win->setClearColor(button_cfg->idle_color);
            SDL_RenderFillRect(win->getRenderer(), &dst);
            win->setClearColor(global_cfg->bg);
        }
    }
    else
    {
        SDL_RenderDrawRect(win->getRenderer(), &dst);
        win->setClearColor(button_cfg->idle_color);
        SDL_RenderFillRect(win->getRenderer(), &dst);
        win->setClearColor(global_cfg->bg);
    }

    if (ctx->hot == id && ctx->active == id)
        return 1;
    return 0;
}

} // namespace tnt::ImGui

// TODO: narrow this to only needed things.

/////////////////
// tnt_imgui_* //
/////////////////

void tnt_imgui_begin(tnt::ImGui::context_t *ctx) noexcept
{
    ctx->hot = 0;
}

void tnt_imgui_finish(tnt::ImGui::context_t *ctx) noexcept
{
    if (!ctx->mouse_down)
        ctx->active = 0;
    else
    {
        if (ctx->active == 0)
            ctx->active = -1;
    }
}
