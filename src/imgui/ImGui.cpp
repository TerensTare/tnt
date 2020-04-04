// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <SDL2/SDL_events.h>

#include "imgui/ImGui.hpp"
#include "imgui/gui_config.hpp"

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

struct slider_config
{
    int w;
    int h;
    int thumb_w;
    int thumb_h;

    SDL_Color color;
    SDL_Color thumb_idle_color;
    SDL_Color thumb_active_color;
} *slider_cfg = {new slider_config{
    .w = 20,
    .h = 100,
    .thumb_w = 18,
    .thumb_h = 18,
    .color = SDL_Color{100, 100, 100, 255},
    .thumb_idle_color = SDL_Color{200, 100, 100, 255},
    .thumb_active_color = SDL_Color{20, 110, 200, 255}}};

struct hslider_config
{
    int w;
    int h;
    int thumb_w;
    int thumb_h;

    SDL_Color color;
    SDL_Color thumb_idle_color;
    SDL_Color thumb_active_color;
} *hslider_cfg = {new hslider_config{
    .w = 100,
    .h = 20,
    .thumb_w = 18,
    .thumb_h = 18,
    .color = SDL_Color{100, 100, 100, 255},
    .thumb_idle_color = SDL_Color{200, 100, 100, 255},
    .thumb_active_color = SDL_Color{20, 110, 200, 255}}};

struct context_t
{
    int mouse_x, mouse_y;
    bool mouse_down;
    int active, hot;
    SDL_Event event;
} * context{new context_t{}};

////////////
// global //
////////////

int get_window_w() noexcept { return global_cfg->w; }
int get_window_h() noexcept { return global_cfg->h; }

unsigned char *get_bg_color() noexcept
{
    static unsigned char arr[4]{
        global_cfg->bg.r,
        global_cfg->bg.g, global_cfg->bg.b, global_cfg->bg.a};
    return arr;
}

////////////
// button //
////////////

void set_button_w(int w) noexcept { button_cfg->w = w; }
int get_button_w() noexcept { return button_cfg->w; }

void set_button_h(int h) noexcept { button_cfg->h = h; }
int get_button_h() noexcept { return button_cfg->h; }

void set_button_idle_color(
    unsigned char r, unsigned char g,
    unsigned char b, unsigned char a) noexcept
{
    button_cfg->idle_color = SDL_Color{r, g, b, a};
}

unsigned char *get_button_idle_color() noexcept
{
    static unsigned char arr[4]{
        button_cfg->idle_color.r, button_cfg->idle_color.g,
        button_cfg->idle_color.b, button_cfg->idle_color.a};
    return arr;
}

void set_button_active_color(
    unsigned char r, unsigned char g,
    unsigned char b, unsigned char a) noexcept
{
    button_cfg->active_color = SDL_Color{r, g, b, a};
}

unsigned char *get_button_active_color() noexcept
{
    static unsigned char arr[4]{
        button_cfg->active_color.r, button_cfg->active_color.g,
        button_cfg->active_color.b, button_cfg->active_color.a};
    return arr;
}

////////////
// slider //
////////////

void set_slider_w(int w) noexcept { slider_cfg->w = w; }
int get_slider_w() noexcept { return slider_cfg->w; }

void set_slider_h(int h) noexcept { slider_cfg->h = h; }
int get_slider_h() noexcept { return slider_cfg->h; }

void set_slider_thumb_w(int w) noexcept { slider_cfg->thumb_w = w; }
int get_slider_thumb_w() noexcept { return slider_cfg->thumb_w; }

void set_slider_thumb_h(int h) noexcept { slider_cfg->thumb_h = h; }
int get_slider_thumb_h() noexcept { return slider_cfg->thumb_h; }

void set_slider_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) noexcept
{
    slider_cfg->color = SDL_Color{r, g, b, a};
}

unsigned char *get_slider_color() noexcept
{
    static unsigned char arr[4]{
        slider_cfg->color.r, slider_cfg->color.g,
        slider_cfg->color.b, slider_cfg->color.a};
    return arr;
}

void set_slider_thumb_idle_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) noexcept
{
    slider_cfg->thumb_active_color = SDL_Color{r, g, b, a};
}
unsigned char *get_slider_thumb_idle_color() noexcept
{
    static unsigned char arr[4]{
        slider_cfg->thumb_idle_color.r,
        slider_cfg->thumb_idle_color.g,
        slider_cfg->thumb_idle_color.b,
        slider_cfg->thumb_idle_color.a};
    return arr;
}

void set_slider_thumb_active_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) noexcept
{
    slider_cfg->thumb_active_color = SDL_Color{r, g, b, a};
}

unsigned char *get_slider_thumb_active_color() noexcept
{
    static unsigned char arr[4]{
        slider_cfg->thumb_active_color.r,
        slider_cfg->thumb_active_color.g,
        slider_cfg->thumb_active_color.b,
        slider_cfg->thumb_active_color.a};
    return arr;
}

///////////////////////
// horizontal slider //
///////////////////////

void set_hslider_w(int w) noexcept { hslider_cfg->w = w; }
int get_hslider_w() noexcept { return hslider_cfg->w; }

void set_hslider_h(int h) noexcept { hslider_cfg->h = h; }
int get_hslider_h() noexcept { return hslider_cfg->h; }

void set_hslider_thumb_w(int w) noexcept { hslider_cfg->thumb_w = w; }
int get_hslider_thumb_w() noexcept { return hslider_cfg->thumb_w; }

void set_hslider_thumb_h(int h) noexcept { hslider_cfg->thumb_h = h; }
int get_hslider_thumb_h() noexcept { return hslider_cfg->thumb_h; }

void set_hslider_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) noexcept
{
    hslider_cfg->color = SDL_Color{r, g, b, a};
}

unsigned char *get_hslider_color() noexcept
{
    static unsigned char arr[4]{
        hslider_cfg->color.r,
        hslider_cfg->color.g,
        hslider_cfg->color.b,
        hslider_cfg->color.a};
    return arr;
}

void set_hslider_thumb_idle_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) noexcept
{
    hslider_cfg->thumb_active_color = SDL_Color{r, g, b, a};
}
unsigned char *get_hslider_thumb_idle_color() noexcept
{
    static unsigned char arr[4]{
        hslider_cfg->thumb_idle_color.r,
        hslider_cfg->thumb_idle_color.g,
        hslider_cfg->thumb_idle_color.b,
        hslider_cfg->thumb_idle_color.a};
    return arr;
}

void set_hslider_thumb_active_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) noexcept
{
    hslider_cfg->thumb_active_color = SDL_Color{r, g, b, a};
}

unsigned char *get_hslider_thumb_active_color() noexcept
{
    static unsigned char arr[4]{
        hslider_cfg->thumb_active_color.r,
        hslider_cfg->thumb_active_color.g,
        hslider_cfg->thumb_active_color.b,
        hslider_cfg->thumb_active_color.a};
    return arr;
}

/////////////////
// local utils //
/////////////////

auto on_rect = [](int x, int y, int w, int h) -> bool {
    return ((context->mouse_x >= x) && (context->mouse_x < x + w) &&
            (context->mouse_y >= y) && (context->mouse_y < y + h));
};

auto draw_rect = [](Window *win, SDL_Rect const &rect, SDL_Color const &color) -> void {
    SDL_RenderDrawRect(win->getRenderer(), &rect);
    win->setClearColor(color);
    SDL_RenderFillRect(win->getRenderer(), &rect);
    win->setClearColor(global_cfg->bg);
};

///////////////////
// context stuff //
///////////////////

void make_context(int w, int h) noexcept
{
    if (global_cfg->w == 0) // (maybe) make these 0 on destroy_context
        global_cfg->w = w;
    if (global_cfg->h == 0)
        global_cfg->h = h;

    auto pos{InputManager::This().MousePosition()};

    context->mouse_x = pos.first;
    context->mouse_y = pos.second;
    context->mouse_down = InputManager::This().MouseButtonDown(0);
    context->active = 0;
    context->hot = 0;
}

void update_context() noexcept
{
    auto pos{InputManager::This().MousePosition()};

    context->mouse_x = pos.first;
    context->mouse_y = pos.second;
    context->mouse_down = InputManager::This().MouseButtonDown(0);
    SDL_PollEvent(&context->event);
}

void destroy_context() noexcept
{
    delete button_cfg;
    button_cfg = nullptr;

    delete slider_cfg;
    slider_cfg = nullptr;

    delete hslider_cfg;
    hslider_cfg = nullptr;

    delete global_cfg;
    global_cfg = nullptr;

    context->mouse_x = 0;
    context->mouse_y = 0;
    context->mouse_down = false;
    context->active = 0;
    context->hot = 0;

    delete context;
    context = nullptr;
}

/////////////
// widgets //
/////////////

int button(Window *win, int id, int x, int y) noexcept
{
    if (on_rect(x, y, button_cfg->w, button_cfg->h))
    {
        context->hot = id;
        if (context->active == 0 && context->mouse_down) // left pressed
            context->active = id;
    }

    SDL_Rect dst{x, y, button_cfg->w, button_cfg->h};

    SDL_Color widgetColor;

    if (context->hot == id)
    {
        if (context->active == id)
            widgetColor = button_cfg->active_color;
        else
            widgetColor = button_cfg->idle_color;
    }
    else
        widgetColor = button_cfg->idle_color;

    draw_rect(win, dst, widgetColor);

    if (context->hot == id && context->active == id)
        return 1;
    return 0;
}

int slider_int(Window *win, int id, int x, int y,
               int min, int max, int &value) noexcept
{
    int ypos{((slider_cfg->h - slider_cfg->thumb_h) * value) / max};
    int offset{slider_cfg->w - slider_cfg->thumb_w};

    if (on_rect(x, y, slider_cfg->w, slider_cfg->h))
    {
        context->hot = id;
        if (context->active == 0 && context->mouse_down)
            context->active = id;
    }

    draw_rect(win, {x, y, slider_cfg->w, slider_cfg->h + offset},
              slider_cfg->color);

    SDL_Rect thumb{x + (offset / 2), y + (offset / 2) + ypos,
                   slider_cfg->thumb_w, slider_cfg->thumb_h};

    SDL_Color widgetColor;

    if (context->active == id || context->hot == id)
        widgetColor = slider_cfg->thumb_active_color;
    else
        widgetColor = slider_cfg->thumb_idle_color;

    draw_rect(win, thumb, widgetColor);

    if (context->active == id)
    {
        int mousePos{context->mouse_y - (y + (offset / 2))};
        if (mousePos < 0)
            mousePos = 0;
        if (mousePos > (slider_cfg->h - 1))
            mousePos = (slider_cfg->h - 1);
        if (int v{(mousePos * max) / (slider_cfg->h - 1)};
            v != value)
        {
            value = v;
            return 1;
        }
    }
    return 0;
}

int slider_float(
    Window *win, int id, int x, int y,
    float min, float max, float &value) noexcept
{
    int ypos{static_cast<int>(((slider_cfg->h - slider_cfg->thumb_h) * value) / max)};
    int offset{slider_cfg->w - slider_cfg->thumb_w};

    if (on_rect(x, y, slider_cfg->w, slider_cfg->h))
    {
        context->hot = id;
        if (context->active == 0 && context->mouse_down)
            context->active = id;
    }

    draw_rect(win, {x, y, slider_cfg->w, slider_cfg->h + offset},
              slider_cfg->color);

    SDL_Rect thumb{x + (offset / 2), y + (offset / 2) + ypos,
                   slider_cfg->thumb_w, slider_cfg->thumb_h};

    SDL_Color widgetColor;

    if (context->active == id || context->hot == id)
        widgetColor = slider_cfg->thumb_active_color;
    else
        widgetColor = slider_cfg->thumb_idle_color;

    draw_rect(win, thumb, widgetColor);

    if (context->active == id)
    {
        int mousePos{context->mouse_y - (y + (offset / 2))};
        if (mousePos < 0)
            mousePos = 0;
        if (mousePos > (slider_cfg->h - 1))
            mousePos = (slider_cfg->h - 1);
        if (float v{static_cast<float>((mousePos * max) / (slider_cfg->h - 1))};
            v != value)
        {
            value = v;
            return 1;
        }
    }
    return 0;
}

int slider_byte(
    Window *win, int id, int x, int y,
    unsigned char min, unsigned char max, unsigned char &value) noexcept
{
    int ypos{static_cast<int>(((slider_cfg->h - slider_cfg->thumb_h) * value) / max)};
    int offset{slider_cfg->w - slider_cfg->thumb_w};

    if (on_rect(x, y, slider_cfg->w, slider_cfg->h))
    {
        context->hot = id;
        if (context->active == 0 && context->mouse_down)
            context->active = id;
    }

    draw_rect(win, {x, y, slider_cfg->w, slider_cfg->h + offset},
              slider_cfg->color);

    SDL_Rect thumb{x + (offset / 2), y + (offset / 2) + ypos,
                   slider_cfg->thumb_w, slider_cfg->thumb_h};

    SDL_Color widgetColor;

    if (context->active == id || context->hot == id)
        widgetColor = slider_cfg->thumb_active_color;
    else
        widgetColor = slider_cfg->thumb_idle_color;

    draw_rect(win, thumb, widgetColor);

    if (context->active == id)
    {
        int mousePos{context->mouse_y - (y + (offset / 2))};
        if (mousePos < 0)
            mousePos = 0;
        if (mousePos > (slider_cfg->h - 1))
            mousePos = (slider_cfg->h - 1);
        unsigned char v{static_cast<unsigned char>((mousePos * max) / (slider_cfg->h - 1))};
        if (v != value)
        {
            value = v;
            return 1;
        }
    }
    return 0;
}

int hslider_int(Window *win, int id, int x, int y,
                int min, int max, int &value) noexcept
{
    int xpos{((hslider_cfg->w - hslider_cfg->thumb_w) * value) / max};
    int offset{hslider_cfg->h - hslider_cfg->thumb_h};

    if (on_rect(x, y, hslider_cfg->w, hslider_cfg->h))
    {
        context->hot = id;
        if (context->active == 0 && context->mouse_down)
            context->active = id;
    }

    draw_rect(win, {x, y, hslider_cfg->w + offset, hslider_cfg->h},
              hslider_cfg->color);

    SDL_Rect thumb{x + (offset / 2) + xpos, y + (offset / 2),
                   hslider_cfg->thumb_w, hslider_cfg->thumb_h};

    SDL_Color widgetColor;

    if (context->active == id || context->hot == id)
        widgetColor = hslider_cfg->thumb_active_color;
    else
        widgetColor = hslider_cfg->thumb_idle_color;

    draw_rect(win, thumb, widgetColor);

    if (context->active == id)
    {
        int mousePos{context->mouse_x - (x + (offset / 2))};
        if (mousePos < 0)
            mousePos = 0;
        if (mousePos > (hslider_cfg->w - 1))
            mousePos = (hslider_cfg->w - 1);

        if (int v{(mousePos * max) / (hslider_cfg->w - 1)};
            v != value)
        {
            value = v;
            return 1;
        }
    }
    return 0;
}

// TODO: WIP
int hslider_float(
    Window *win, int id, int x, int y,
    float min, float max, float &value) noexcept
{
    int xpos{static_cast<int>(((hslider_cfg->w - hslider_cfg->thumb_w) * value) / max)};
    int offset{hslider_cfg->h - hslider_cfg->thumb_h};

    if (on_rect(x, y, hslider_cfg->w, hslider_cfg->h))
    {
        context->hot = id;
        if (context->active == 0 && context->mouse_down)
            context->active = id;
    }

    draw_rect(win, {x, y, hslider_cfg->w + offset, hslider_cfg->h},
              hslider_cfg->color);

    SDL_Rect thumb{x + (offset / 2) + xpos, y + (offset / 2),
                   hslider_cfg->thumb_w, hslider_cfg->thumb_h};

    SDL_Color widgetColor;

    if (context->active == id || context->hot == id)
        widgetColor = hslider_cfg->thumb_active_color;
    else
        widgetColor = hslider_cfg->thumb_idle_color;

    draw_rect(win, thumb, widgetColor);

    if (context->active == id)
    {
        int mousePos{context->mouse_x - (x + (offset / 2))};
        if (mousePos < 0)
            mousePos = 0;
        if (mousePos > (hslider_cfg->w - 1))
            mousePos = (hslider_cfg->w - 1);

        if (float v{static_cast<float>((mousePos * max) / (hslider_cfg->w - 1))};
            v != value)
        {
            value = v;
            return 1;
        }
    }
    return 0;
}

// TODO: WIP
int hslider_byte(
    Window *win, int id, int x, int y,
    unsigned char min, unsigned char max, unsigned char &value) noexcept
{
    int ypos{static_cast<int>(((slider_cfg->h - slider_cfg->thumb_h) * value) / max)};
    int offset{slider_cfg->w - slider_cfg->thumb_w};

    if (on_rect(x, y, slider_cfg->w, slider_cfg->h))
    {
        context->hot = id;
        if (context->active == 0 && context->mouse_down)
            context->active = id;
    }

    draw_rect(win, {x, y, slider_cfg->w, slider_cfg->h + offset},
              slider_cfg->color);

    SDL_Rect thumb{x + (offset / 2), y + (offset / 2) + ypos,
                   slider_cfg->thumb_w, slider_cfg->thumb_h};

    SDL_Color widgetColor;

    if (context->active == id || context->hot == id)
        widgetColor = slider_cfg->thumb_active_color;
    else
        widgetColor = slider_cfg->thumb_idle_color;

    draw_rect(win, thumb, widgetColor);

    if (context->active == id)
    {
        int mousePos{context->mouse_y - (y + (offset / 2))};
        if (mousePos < 0)
            mousePos = 0;
        if (mousePos > (slider_cfg->h - 1))
            mousePos = (slider_cfg->h - 1);
        unsigned char v{static_cast<unsigned char>((mousePos * max) / (slider_cfg->h - 1))};
        if (v != value)
        {
            value = v;
            return 1;
        }
    }
    return 0;
}
} // namespace tnt::ImGui

/////////////////
// tnt_imgui_* //
/////////////////

void tnt_imgui_begin() noexcept
{
    tnt::ImGui::context->hot = 0;
}

void tnt_imgui_finish() noexcept
{
    if (!tnt::ImGui::context->mouse_down)
        tnt::ImGui::context->active = 0;
    else
    {
        if (tnt::ImGui::context->active == 0)
            tnt::ImGui::context->active = -1;
    }
}
