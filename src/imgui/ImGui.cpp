// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <map>

#include "core/InputManager.hpp"
#include "core/Window.hpp"
#include "imgui/ImGui.hpp"
#include "imgui/gui_config.hpp"
#include "utils/Logger.hpp"

namespace tnt::ImGui
{
struct global_config
{
    int w;
    int h;
    int font_size;
    SDL_Color bg;
    SDL_Color text_color;
    std::string default_font;
    TTF_Font *font_data;
} * global_cfg{new global_config{
        .w = 0,
        .h = 0,
        .font_size = 14,
        .text_color = SDL_Color{255, 255, 255, 255},
        .default_font = std::string{SDL_GetBasePath()}.append("assets\\zeldadxt.ttf")}};

struct button_config
{
    int w;
    int h;

    SDL_Color idle_color;
    SDL_Color active_color;
    SDL_Texture *text;
} * button_cfg{new button_config{.w = 60,
                                 .h = 40,
                                 .idle_color = SDL_Color{30, 144, 255, 255},
                                 .active_color = SDL_Color{30, 144, 255, 255}}};

struct slider_config
{
    int w;
    int h;
    int thumb_w;
    int thumb_h;

    SDL_Color color;
    SDL_Color thumb_idle_color;
    SDL_Color thumb_active_color;
} *slider_cfg = {new slider_config{.w = 20,
                                   .h = 100,
                                   .thumb_w = 18,
                                   .thumb_h = 18,
                                   .color = SDL_Color{100, 100, 100, 255},
                                   .thumb_idle_color = SDL_Color{30, 144, 255, 255},
                                   .thumb_active_color = SDL_Color{30, 144, 255, 255}}};

struct hslider_config
{
    int w;
    int h;
    int thumb_w;
    int thumb_h;

    SDL_Color color;
    SDL_Color thumb_idle_color;
    SDL_Color thumb_active_color;
} *hslider_cfg = {new hslider_config{.w = 100,
                                     .h = 20,
                                     .thumb_w = 18,
                                     .thumb_h = 18,
                                     .color = SDL_Color{100, 100, 100, 255},
                                     .thumb_idle_color = SDL_Color{30, 144, 255, 255},
                                     .thumb_active_color = SDL_Color{30, 144, 255, 255}}};

struct progress_bar_config
{
    int w;
    int h;

    SDL_Color idle_color;
    SDL_Color filled_color;
} * progress_bar_cfg{new progress_bar_config{.w = 100,
                                             .h = 20,
                                             .idle_color = SDL_Color{100, 100, 100, 255},
                                             .filled_color = SDL_Color{30, 144, 255, 255}}};

struct window_config
{
    int w, h;
    SDL_Color bg;
    SDL_Color titlebar_color;
} * window_cfg{new window_config{.w = 400,
                                 .h = 300,
                                 .bg = SDL_Color{0, 0, 0},
                                 .titlebar_color = SDL_Color{30, 144, 255}}};

struct checkbox_config
{
    int length; // w, h
} * checkbox_cfg{new checkbox_config{.length = 10}};

struct menu_config
{
    int spacing;
    int w, h;
} * menu_cfg{new menu_config{.spacing = 50, .w = window_cfg->w, .h = 20}};

struct context_t
{
    // this is true if calling Begin() inside other Begin/End pair.
    bool on_window;
    bool mouse_down;
    int mouse_x, mouse_y;
    long long active, hot;
    std::string last_window;
    std::map<std::string, SDL_Texture *> windows;
} * context{new context_t{}};

#ifdef TNT_IMGUI_RUNTIME_CONFIG

////////////
// global //
////////////

int get_window_w() noexcept { return global_cfg->w; }
int get_window_h() noexcept { return global_cfg->h; }

unsigned char *get_bg_color() noexcept
{
    static unsigned char arr[4]{global_cfg->bg.r, global_cfg->bg.g, global_cfg->bg.b,
                                global_cfg->bg.a};
    return arr;
}

unsigned char *get_font_color() noexcept
{
    static unsigned char arr[4]{global_cfg->text_color.r, global_cfg->text_color.g,
                                global_cfg->text_color.b, global_cfg->text_color.a};
    return arr;
}

// TODO: load the other font texture here.
void set_font(char const *name) noexcept
{
    global_cfg->default_font = name;
    TTF_CloseFont(global_cfg->font_data);
    global_cfg->font_data = TTF_OpenFont(name, global_cfg->font_size);
}

TTF_Font *get_font() noexcept { return global_cfg->font_data; }

void set_font_size(int size) noexcept { global_cfg->font_size = size; }

int get_font_size() noexcept { return global_cfg->font_size; }

////////////
// button //
////////////

void set_button_w(int w) noexcept { button_cfg->w = w; }
int get_button_w() noexcept { return button_cfg->w; }

void set_button_h(int h) noexcept { button_cfg->h = h; }
int get_button_h() noexcept { return button_cfg->h; }

void set_button_idle_color(unsigned char r, unsigned char g, unsigned char b,
                           unsigned char a) noexcept
{
    button_cfg->idle_color = SDL_Color{r, g, b, a};
}

unsigned char *get_button_idle_color() noexcept
{
    static unsigned char arr[4]{button_cfg->idle_color.r, button_cfg->idle_color.g,
                                button_cfg->idle_color.b, button_cfg->idle_color.a};
    return arr;
}

void set_button_active_color(unsigned char r, unsigned char g, unsigned char b,
                             unsigned char a) noexcept
{
    button_cfg->active_color = SDL_Color{r, g, b, a};
}

unsigned char *get_button_active_color() noexcept
{
    static unsigned char arr[4]{button_cfg->active_color.r, button_cfg->active_color.g,
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

void set_slider_color(unsigned char r, unsigned char g, unsigned char b,
                      unsigned char a) noexcept
{
    slider_cfg->color = SDL_Color{r, g, b, a};
}

unsigned char *get_slider_color() noexcept
{
    static unsigned char arr[4]{slider_cfg->color.r, slider_cfg->color.g, slider_cfg->color.b,
                                slider_cfg->color.a};
    return arr;
}

void set_slider_thumb_idle_color(unsigned char r, unsigned char g, unsigned char b,
                                 unsigned char a) noexcept
{
    slider_cfg->thumb_idle_color = SDL_Color{r, g, b, a};
}
unsigned char *get_slider_thumb_idle_color() noexcept
{
    static unsigned char arr[4]{slider_cfg->thumb_idle_color.r, slider_cfg->thumb_idle_color.g,
                                slider_cfg->thumb_idle_color.b, slider_cfg->thumb_idle_color.a};
    return arr;
}

void set_slider_thumb_active_color(unsigned char r, unsigned char g, unsigned char b,
                                   unsigned char a) noexcept
{
    slider_cfg->thumb_active_color = SDL_Color{r, g, b, a};
}

unsigned char *get_slider_thumb_active_color() noexcept
{
    static unsigned char arr[4]{
        slider_cfg->thumb_active_color.r, slider_cfg->thumb_active_color.g,
        slider_cfg->thumb_active_color.b, slider_cfg->thumb_active_color.a};
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

void set_hslider_color(unsigned char r, unsigned char g, unsigned char b,
                       unsigned char a) noexcept
{
    hslider_cfg->color = SDL_Color{r, g, b, a};
}

unsigned char *get_hslider_color() noexcept
{
    static unsigned char arr[4]{hslider_cfg->color.r, hslider_cfg->color.g,
                                hslider_cfg->color.b, hslider_cfg->color.a};
    return arr;
}

void set_hslider_thumb_idle_color(unsigned char r, unsigned char g, unsigned char b,
                                  unsigned char a) noexcept
{
    hslider_cfg->thumb_idle_color = SDL_Color{r, g, b, a};
}
unsigned char *get_hslider_thumb_idle_color() noexcept
{
    static unsigned char arr[4]{
        hslider_cfg->thumb_idle_color.r, hslider_cfg->thumb_idle_color.g,
        hslider_cfg->thumb_idle_color.b, hslider_cfg->thumb_idle_color.a};
    return arr;
}

void set_hslider_thumb_active_color(unsigned char r, unsigned char g, unsigned char b,
                                    unsigned char a) noexcept
{
    hslider_cfg->thumb_active_color = SDL_Color{r, g, b, a};
}

unsigned char *get_hslider_thumb_active_color() noexcept
{
    static unsigned char arr[4]{
        hslider_cfg->thumb_active_color.r, hslider_cfg->thumb_active_color.g,
        hslider_cfg->thumb_active_color.b, hslider_cfg->thumb_active_color.a};
    return arr;
}

//////////////////
// progress bar //
//////////////////

int get_progressbar_w() noexcept { return progress_bar_cfg->w; }
void set_progressbar_w(int w) noexcept { progress_bar_cfg->w = w; }

int get_progressbar_h() noexcept { return progress_bar_cfg->h; }
void set_progressbar_h(int h) noexcept { progress_bar_cfg->h = h; }

unsigned char *get_progressbar_idle_color() noexcept
{
    static unsigned char arr[4]{progress_bar_cfg->idle_color.r, progress_bar_cfg->idle_color.g,
                                progress_bar_cfg->idle_color.b, progress_bar_cfg->idle_color.a};
    return arr;
}

void set_progressbar_idle_color(unsigned char r, unsigned char g, unsigned char b,
                                unsigned char a) noexcept
{
    progress_bar_cfg->idle_color = SDL_Color{r, g, b, a};
}

unsigned char *get_progressbar_filled_color() noexcept
{
    static unsigned char arr[4]{
        progress_bar_cfg->filled_color.r, progress_bar_cfg->filled_color.g,
        progress_bar_cfg->filled_color.b, progress_bar_cfg->filled_color.a};
    return arr;
}

void set_progressbar_filled_color(unsigned char r, unsigned char g, unsigned char b,
                                  unsigned char a) noexcept
{
    progress_bar_cfg->filled_color = SDL_Color{r, g, b, a};
}

//////////////
// checkbox //
//////////////

int get_checkbox_length() noexcept { return checkbox_cfg->length; }
void set_checkbox_length(int len) noexcept { checkbox_cfg->length = len; }

#endif //!TNT_IMGUI_RUNTIME_CONFIG

/////////////////
// local utils //
/////////////////

auto on_rect = [](int x, int y, int w, int h) -> bool {
    return ((context->mouse_x >= x) && (context->mouse_x < x + w) && (context->mouse_y >= y) &&
            (context->mouse_y < y + h));
};

auto load_text = [](Window *win, char const *text,
                    SDL_Color color = global_cfg->text_color) -> SDL_Texture * {
    SDL_Surface *temp{TTF_RenderText_Blended(global_cfg->font_data, text, color)};
    if (temp == nullptr)
    {
        tnt::logger::debug("Couldn't load surface for text {} with size {}!!", text,
                           global_cfg->font_size);
        return nullptr;
    }

    SDL_Texture *ret{SDL_CreateTextureFromSurface(win->getRenderer(), temp)};

    SDL_FreeSurface(temp);
    temp = nullptr;

    if (ret == nullptr)
    {
        tnt::logger::debug("Couldn't load font texture for text {} with size {}!!", text,
                           global_cfg->font_size);
        return nullptr;
    }

    return ret;
};

auto draw_rect = [](Window *win, SDL_Rect const &rect, SDL_Color const &color) -> void {
    SDL_RenderDrawRect(win->getRenderer(), &rect);
    win->setClearColor(color);
    SDL_RenderFillRect(win->getRenderer(), &rect);
    win->setClearColor(global_cfg->bg);
};

auto draw_line = [](Window *win, int x1, int y1, int x2, int y2, SDL_Color const &color) -> void {
    win->setClearColor(color);
    SDL_RenderDrawLine(win->getRenderer(), x1, y1, x2, y2);
    win->setClearColor(global_cfg->bg);
};

auto draw_text = [](Window *win, char const *text, int x, int y,
                    SDL_Color color = global_cfg->text_color,
                    int size = global_cfg->font_size) -> void {
    SDL_Texture *cache{load_text(win, text, color)};
    int w{0}, h{0};
    SDL_QueryTexture(cache, nullptr, nullptr, &w, &h);
    SDL_Rect where{x, y, w * size / global_cfg->font_size, h * size / global_cfg->font_size};
    SDL_RenderCopy(win->getRenderer(), cache, nullptr, &where);
    SDL_DestroyTexture(cache);
    cache = nullptr;
};

auto has_flag = [](WindowFlags owner, WindowFlags test) -> bool {
    return ((owner & test) == test);
};

///////////////////
// context stuff //
///////////////////

void make_context() noexcept
{
    auto pos{input::mousePosition()};

    context->mouse_x = pos.first;
    context->mouse_y = pos.second;
    context->on_window = false;
    context->mouse_down = input::mouseButtonDown(0);
    context->active = 0;
    context->hot = 0;
    context->last_window.clear();
}

void update_context() noexcept
{
    tnt::input::updatePrevious();
    tnt::input::updateCurrent();

    auto pos{input::mousePosition()};

    context->mouse_x = pos.first;
    context->mouse_y = pos.second;
    context->mouse_down = input::mouseButtonDown(0);
}

void destroy_context() noexcept
{
    context->mouse_x = 0;
    context->mouse_y = 0;
    context->mouse_down = false;
    context->active = 0;
    context->hot = 0;

    for (auto const &it : context->windows)
        if (it.second != nullptr)
            SDL_DestroyTexture(it.second);

    delete context;
    context = nullptr;
}

/////////////
// widgets //
/////////////

bool Begin(Window *win, std::string_view name, int x, int y, WindowFlags flags) noexcept
{
    if (context->on_window)
    {
        tnt::logger::debug(
            "Line : {}\tCalling tnt::ImGui::Begin() inside a tnt::ImGui::Begin()/tnt::ImGui::End() pair!!",
            __LINE__);
        return false;
    }

    context->windows.emplace(name.data(), load_text(win, name.data()));
    context->last_window = name;
    context->on_window = true;
    context->hot = 0;

    // if (has_flag(flags, WindowFlags::Default))
    // {
    draw_rect(win, {x, y, window_cfg->w, window_cfg->h}, window_cfg->bg);  // body
    draw_rect(win, {x, y, window_cfg->w, 20}, window_cfg->titlebar_color); // title bar
    draw_text(win, name.data(), x + 20, y + 5);                            // title
    // }
    // else
    // {}

    return true;
}

void End() noexcept
{
    if (!context->on_window)
    {
        tnt::logger::debug(
            "Line : {}\tCalling tnt::ImGui::End() without calling tnt::ImGui::Begin() before!!",
            __LINE__);
        return;
    }

    if (!context->mouse_down)
        context->active = 0;
    else
    {
        if (context->active == 0)
            context->active = -1;
    }

    context->on_window = false;
}

bool button(Window *win, long long id, std::string_view text, int x, int y) noexcept
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
    draw_text(win, text.data(), x + 6, y + ((button_cfg->h - global_cfg->font_size) / 2),
              global_cfg->text_color, font_size);

    if (context->hot == id && context->active == id)
        return true;
    return false;
}

bool slider_int(Window *win, long long id, int x, int y, int min_, int max_,
                int *value) noexcept
{
    int ypos{((slider_cfg->h - slider_cfg->thumb_h) * (*value - min_)) / (max_ - min_)};
    int offset{slider_cfg->w - slider_cfg->thumb_w};

    if (on_rect(x, y, slider_cfg->w, slider_cfg->h))
    {
        context->hot = id;
        if (context->active == 0 && context->mouse_down)
            context->active = id;
    }

    draw_rect(win, {x, y, slider_cfg->w, slider_cfg->h + offset}, slider_cfg->color);

    SDL_Rect thumb{x + (offset / 2), y + (offset / 2) + ypos, slider_cfg->thumb_w,
                   slider_cfg->thumb_h};

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
        if (int v{(mousePos * max_) / (slider_cfg->h - 1)}; v != *value)
        {
            *value = v;
            return true;
        }
    }
    return false;
}

bool slider_float(Window *win, long long id, int x, int y, float min_, float max_,
                  float *value) noexcept
{
    int ypos{static_cast<int>(((slider_cfg->h - slider_cfg->thumb_h) * (*value - min_)) /
                              (max_ - min_))};
    int offset{slider_cfg->w - slider_cfg->thumb_w};

    if (on_rect(x, y, slider_cfg->w, slider_cfg->h))
    {
        context->hot = id;
        if (context->active == 0 && context->mouse_down)
            context->active = id;
    }

    draw_rect(win, {x, y, slider_cfg->w, slider_cfg->h + offset}, slider_cfg->color);

    SDL_Rect thumb{x + (offset / 2), y + (offset / 2) + ypos, slider_cfg->thumb_w,
                   slider_cfg->thumb_h};

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
        if (float v{static_cast<float>((mousePos * max_) / (slider_cfg->h - 1))}; v != *value)
        {
            *value = v;
            return true;
        }
    }
    return false;
}

bool hslider_int(Window *win, long long id, int x, int y, int min_, int max_,
                 int *value) noexcept
{
    int xpos{((hslider_cfg->w - hslider_cfg->thumb_w) * (*value - min_) / (max_ - min_))};
    int offset{hslider_cfg->h - hslider_cfg->thumb_h};

    if (on_rect(x, y, hslider_cfg->w, hslider_cfg->h))
    {
        context->hot = id;
        if (context->active == 0 && context->mouse_down)
            context->active = id;
    }

    draw_rect(win, {x, y, hslider_cfg->w + offset, hslider_cfg->h}, hslider_cfg->color);

    SDL_Rect thumb{x + (offset / 2) + xpos, y + (offset / 2), hslider_cfg->thumb_w,
                   hslider_cfg->thumb_h};

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

        if (int v{min_ + (mousePos * (max_ - min_)) / (hslider_cfg->w - 1)}; v != *value)
        {
            *value = v;
            return true;
        }
    }
    return false;
}

bool hslider_float(Window *win, long long id, int x, int y, float min_, float max_,
                   float *value) noexcept
{
    int xpos{static_cast<int>(((hslider_cfg->w - hslider_cfg->thumb_w) * (*value - min_)) /
                              (max_ - min_))};
    int offset{hslider_cfg->h - hslider_cfg->thumb_h};

    if (on_rect(x, y, hslider_cfg->w, hslider_cfg->h))
    {
        context->hot = id;
        if (context->active == 0 && context->mouse_down)
            context->active = id;
    }

    draw_rect(win, {x, y, hslider_cfg->w + offset, hslider_cfg->h}, hslider_cfg->color);

    SDL_Rect thumb{x + (offset / 2) + xpos, y + (offset / 2), hslider_cfg->thumb_w,
                   hslider_cfg->thumb_h};

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

        if (float v{static_cast<float>((mousePos * max_) / (hslider_cfg->w - 1))}; v != *value)
        {
            *value = v;
            return true;
        }
    }
    return false;
}

int menu(Window *win, long long id, int x, int y, std::string *first,
         std::string *last) noexcept
{
    for (int i{0}; i < (last - first) + 1; ++i)
        draw_text(win, (first + i)->c_str(), x + 5 + (menu_cfg->spacing * i), y);
    return 1;
}

void progress_bar(Window *win, long long id, int x, int y, int min_, int max_,
                  int *value) noexcept
{
    int xpos{(progress_bar_cfg->w * (*value - min_) / (max_ - min_))};
    draw_rect(win, {x, y, progress_bar_cfg->w, progress_bar_cfg->h},
              progress_bar_cfg->idle_color);
    draw_rect(win, {x, y, xpos, progress_bar_cfg->h}, progress_bar_cfg->filled_color);
}

bool checkbox(Window *win, long long id, int x, int y, bool *value) noexcept
{
    if (on_rect(x, y, checkbox_cfg->length, checkbox_cfg->length))
    {
        context->hot = id;
        if (context->active == 0 && context->mouse_down) // left pressed
            context->active = id;
    }

    draw_rect(win, {x, y, checkbox_cfg->length, checkbox_cfg->length}, {255, 255, 255});

    if (context->hot == id && context->active == id)
        *value = !(*value);

    if (*value)
    {
        draw_line(win, x, y, x + (checkbox_cfg->length / 2),
                  y + (checkbox_cfg->length / 2), {30, 144, 255});
        draw_line(win, x + (checkbox_cfg->length / 2), y + (checkbox_cfg->length / 2),
                  x + checkbox_cfg->length, y, {30, 144, 255});
        return true;
    }

    return false;
}
} // namespace tnt::ImGui

/////////////////
// tnt_imgui_* //
/////////////////

void tnt_imgui_init(tnt::Window *win) noexcept
{
    if (tnt::ImGui::global_cfg->w == 0)
        tnt::ImGui::global_cfg->w = win->getWidth();
    if (tnt::ImGui::global_cfg->h == 0)
        tnt::ImGui::global_cfg->h = win->getHeight();

    tnt::ImGui::global_cfg->font_data = TTF_OpenFont(tnt::ImGui::global_cfg->default_font.data(),
                                                     tnt::ImGui::global_cfg->font_size);
    tnt::ImGui::global_cfg->bg = win->getClearColor();

    tnt::ImGui::make_context();
}

void tnt_imgui_close() noexcept
{
    SDL_DestroyTexture(tnt::ImGui::button_cfg->text);
    tnt::ImGui::button_cfg->text = nullptr;

    delete tnt::ImGui::button_cfg;
    tnt::ImGui::button_cfg = nullptr;

    delete tnt::ImGui::slider_cfg;
    tnt::ImGui::slider_cfg = nullptr;

    delete tnt::ImGui::hslider_cfg;
    tnt::ImGui::hslider_cfg = nullptr;

    delete tnt::ImGui::progress_bar_cfg;
    tnt::ImGui::progress_bar_cfg = nullptr;

    delete tnt::ImGui::menu_cfg;
    tnt::ImGui::menu_cfg = nullptr;

    delete tnt::ImGui::checkbox_cfg;
    tnt::ImGui::checkbox_cfg = nullptr;

    TTF_CloseFont(tnt::ImGui::global_cfg->font_data);
    tnt::ImGui::global_cfg->font_data = nullptr;

    delete tnt::ImGui::global_cfg;
    tnt::ImGui::global_cfg = nullptr;

    tnt::ImGui::destroy_context();
}