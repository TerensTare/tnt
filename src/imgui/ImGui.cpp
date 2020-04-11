// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <SDL2/SDL_image.h>

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
    int font_size;
    SDL_Color bg;
    std::string default_font;
    SDL_Texture *font_tex;
} * global_cfg{new global_config{
        .w = 0,
        .h = 0,
        .font_size = 14,
        .default_font = ".\\bin\\x64\\release\\zeldadxt.png"}};

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

// TODO: load the other font texture here
void set_font(char const *name) noexcept { global_cfg->default_font = name; }
char const *get_font() noexcept { return global_cfg->default_font.c_str(); }

void set_font_size(int size) noexcept { global_cfg->font_size = size; }
int get_font_size() noexcept { return global_cfg->font_size; }

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
    slider_cfg->thumb_idle_color = SDL_Color{r, g, b, a};
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
    hslider_cfg->thumb_idle_color = SDL_Color{r, g, b, a};
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

auto draw_char = [](Window *win, char letter, int x, int y, int size = global_cfg->font_size) -> void {
    SDL_Rect from;
    int delta{static_cast<int>(letter) - 33};

    from.x = (delta % 10);
    from.y = ((delta - from.x) / 10);

    from.x = from.x * global_cfg->font_size;
    from.y = from.y * global_cfg->font_size;

    from.w = global_cfg->font_size;
    from.h = global_cfg->font_size;
    SDL_Rect where{x, y, size, size};
    SDL_RenderCopy(win->getRenderer(), global_cfg->font_tex, &from, &where);
};

auto draw_text = [](Window *win, std::string_view text, int x, int y, int size = global_cfg->font_size) -> void {
    for (auto it{text.begin()}; it != text.end(); ++it)
    {
        draw_char(win, *it, x, y, size);
        x = x + size;
    }
};

///////////////////
// context stuff //
///////////////////

void make_context(Window *win) noexcept
{
    if (global_cfg->w == 0) // (maybe) make these 0 on destroy_context
        global_cfg->w = win->getWidth();
    if (global_cfg->h == 0)
        global_cfg->h = win->getHeight();

    global_cfg->font_tex = IMG_LoadTexture(win->getRenderer(), global_cfg->default_font.c_str());
    SDL_SetTextureBlendMode(global_cfg->font_tex, SDL_BLENDMODE_BLEND);

    global_cfg->bg = win->getClearColor();

    auto pos{input::mousePosition()};

    context->mouse_x = pos.first;
    context->mouse_y = pos.second;
    context->mouse_down = input::mouseButtonDown(0);
    context->active = 0;
    context->hot = 0;
}

void update_context() noexcept
{
    auto pos{input::mousePosition()};

    context->mouse_x = pos.first;
    context->mouse_y = pos.second;
    context->mouse_down = input::mouseButtonDown(0);
}

void destroy_context() noexcept
{
    delete button_cfg;
    button_cfg = nullptr;

    delete slider_cfg;
    slider_cfg = nullptr;

    delete hslider_cfg;
    hslider_cfg = nullptr;

    SDL_DestroyTexture(global_cfg->font_tex);
    global_cfg->font_tex = nullptr;

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

int button(Window *win, int id, std::string_view text, int x, int y) noexcept
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

    int font_size{(button_cfg->w - 10) / static_cast<int>(text.size())};
    draw_text(win, text, x + 5, y + (button_cfg->h / 2), font_size);

    if (context->hot == id && context->active == id)
        return 1;
    return 0;
}

int slider_int(Window *win, int id, int x, int y,
               int min_, int max_, int *value) noexcept
{
    int ypos{((slider_cfg->h - slider_cfg->thumb_h) * (*value - min_)) / (max_ - min_)};
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
        if (int v{(mousePos * max_) / (slider_cfg->h - 1)};
            v != *value)
        {
            *value = v;
            return 1;
        }
    }
    return 0;
}

int slider_float(
    Window *win, int id, int x, int y,
    float min_, float max_, float *value) noexcept
{
    int ypos{static_cast<int>(((slider_cfg->h - slider_cfg->thumb_h) * (*value - min_)) / (max_ - min_))};
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
        if (float v{static_cast<float>((mousePos * max_) / (slider_cfg->h - 1))};
            v != *value)
        {
            *value = v;
            return 1;
        }
    }
    return 0;
}

// TODO: WIP
int slider_byte(
    Window *win, int id, int x, int y,
    unsigned char min_, unsigned char max_, unsigned char *value) noexcept
{
    int ypos{static_cast<int>(((slider_cfg->h - slider_cfg->thumb_h) * (*value - min_)) / (max_ - min_))};
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
        unsigned char v{static_cast<unsigned char>((mousePos * max_) / (slider_cfg->h - 1))};
        if (v != *value)
        {
            *value = v;
            return 1;
        }
    }
    return 0;
}

int hslider_int(Window *win, int id, int x, int y,
                int min_, int max_, int *value) noexcept
{
    int xpos{((hslider_cfg->w - hslider_cfg->thumb_w) * (*value - min_) / (max_ - min_))};
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

        if (int v{min_ + (mousePos * (max_ - min_)) / (hslider_cfg->w - 1)};
            v != *value)
        {
            *value = v;
            return 1;
        }
    }
    return 0;
}

int hslider_float(
    Window *win, int id, int x, int y,
    float min_, float max_, float *value) noexcept
{
    int xpos{static_cast<int>(((hslider_cfg->w - hslider_cfg->thumb_w) * (*value - min_)) / (max_ - min_))};
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

        if (float v{static_cast<float>((mousePos * max_) / (hslider_cfg->w - 1))};
            v != *value)
        {
            *value = v;
            return 1;
        }
    }
    return 0;
}

// TODO: WIP
int hslider_byte(
    Window *win, int id, int x, int y,
    unsigned char min_, unsigned char max_, unsigned char *value) noexcept
{
    int ypos{static_cast<int>(((slider_cfg->h - slider_cfg->thumb_h) * (*value - min_)) / (max_ - min_))};
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
        unsigned char v{static_cast<unsigned char>((mousePos * max_) / (slider_cfg->h - 1))};
        if (v != *value)
        {
            *value = v;
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
