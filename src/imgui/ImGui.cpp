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
        .default_font = std::string{SDL_GetBasePath()}.append("assets/Inconsolata.ttf")}};

struct button_config
{
    int w;
    int h;

    SDL_Color idle_color;
    SDL_Color active_color;
    SDL_Texture *text;
} * button_cfg{new button_config{.w = 60,
                                 .h = 40,
                                 .idle_color = SDL_Color{130, 130, 130, 255},
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
                                   .color = SDL_Color{130, 130, 130, 255},
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
                                     .color = SDL_Color{130, 130, 130, 255},
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
                                             .idle_color = SDL_Color{130, 130, 130, 255},
                                             .filled_color = SDL_Color{30, 144, 255, 255}}};

struct checkbox_config
{
    int length; // w, h
    SDL_Texture *tex;
} * checkbox_cfg{new checkbox_config{.length = 20}};

struct menu_config
{
    int spacing;
} * menu_cfg{new menu_config{.spacing = 20}};

struct window_config
{
    SDL_Color bg;
    SDL_Color titlebar_color;
} * window_cfg{new window_config{
        .bg = SDL_Color{0, 0, 0},
        .titlebar_color = SDL_Color{30, 144, 255}}};

struct window_data
{
    bool menu_called;
    int x, y, w, h;
    int next_y; // next_y is used to arrange widgets on the window.
    WindowFlags win_flags;
    std::string title;
};

struct context_t
{
    bool on_window; // this is true if calling Begin() inside other Begin/End pair.
    bool mouse_down;
    int mouse_x, mouse_y;
    int list_indent_level, list_index, list_number;
    std::size_t active, hot;
    // std::string hash_prefix; // added to each widget's label when hashing
    window_data *last_window;
    std::map<std::string, SDL_Texture *> lists_text;
    std::map<std::string, window_data *> windows;
} * context{new context_t{}};

#define TNT_IMGUI_RUNTIME_CONFIG

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
    if (global_cfg->font_data != nullptr)
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

auto load_image = [](Window *win, char const *path) -> SDL_Texture * {
    return IMG_LoadTexture(win->getRenderer(), path);
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

auto check_button = [](std::size_t id, int x, int y, int w, int h) {
    if (on_rect(x, y, w, h))
    {
        context->hot = id;
        if (context->active == 0 && context->mouse_down) // left pressed
            context->active = id;
    }
};

// thx Robert Gould
// https://stackoverflow.com/questions/628790/have-a-good-hash-function-for-a-c-hash-table
auto im_hash = [](std::string_view str) {
    return ((*(std::size_t *)str.data()) >> 4);
};

///////////////////
// context stuff //
///////////////////

void make_context() noexcept
{
    auto pos{input::mousePosition()};

    context->on_window = false;
    context->mouse_down = input::mouseButtonDown(0);
    context->mouse_y = pos.second;
    context->mouse_x = pos.first;
    context->active = 0;
    context->hot = 0;
}

void update_context() noexcept
{
    tnt::input::updatePrevious();
    tnt::input::updateCurrent();

    context->mouse_down = input::mouseButtonDown(0);
}

void destroy_context() noexcept
{
    context->mouse_down = false;
    context->mouse_x = 0;
    context->mouse_y = 0;
    context->active = 0;
    context->hot = 0;
    context->list_indent_level = 0;
    context->list_index = 0;
    context->list_number = 0;

    for (auto const &it : context->lists_text)
        if (it.second != nullptr)
            SDL_DestroyTexture(it.second);

    for (auto const &it : context->windows)
        if (it.second != nullptr)
            delete it.second;

    if (context->last_window != nullptr)
    {
        delete context->last_window;
        context->last_window = nullptr;
    }

    delete context;
    context = nullptr;
}

/////////////
// widgets //
/////////////

bool Begin(Window *win, std::string_view name, int x_, int y_, WindowFlags flags) noexcept
{
    if (context->on_window)
    {
        tnt::logger::debug(
            "Line : {}\tCalling tnt::ImGui::Begin() inside a tnt::ImGui::Begin()/tnt::ImGui::End() pair!!",
            __LINE__);
        return false;
    }

    if (context->windows.find(name.data()) == context->windows.end())
    {
        if (context->last_window == nullptr)
            context->last_window = new window_data{};

        context->last_window->menu_called = false;
        context->last_window->x = x_;
        context->last_window->y = y_;
        context->last_window->w = 400;
        context->last_window->h = 300;
        context->last_window->win_flags = flags;
        context->last_window->title = name;

        context->windows.emplace(name.data(), context->last_window);
    }

    window_data *current{context->last_window};

    context->on_window = true;

    context->hot = 0;
    context->list_indent_level = 0;
    context->list_index = 0;

    std::size_t id{im_hash(name)};

    std::size_t resize_left_id{im_hash("ResizeLeft")};
    std::size_t resize_right_id{im_hash("ResizeRight")};
    std::size_t resize_down_id{im_hash("ResizeDown")};

    std::size_t resize_right_down_id{im_hash("ResizeRightDown")};
    std::size_t resize_left_down_id{im_hash("ResizeLeftDown")};

    std::pair pos{input::mousePosition()};

    // moving
    check_button(id, current->x, current->y, current->w, 20);
    if (context->active == id)
    {
        current->x = current->x + pos.first - context->mouse_x;
        current->y = current->y + pos.second - context->mouse_y;
    }

    // resizing
    check_button(resize_right_id, current->x + current->w - 10,
                 current->y + 20, 10, current->h - 10);
    check_button(resize_down_id, current->x + 10,
                 current->y + current->h - 10, current->w - 20, 10);
    check_button(resize_right_down_id, current->x + current->w - 10,
                 current->y + current->h - 10, 10, 10);

    if (context->active == resize_right_id)
        if (int dx{pos.first - context->mouse_x}; current->w >= 40 || dx >= 0) // bigger than double of the height of the title bar
            current->w = current->w + dx;
    if (context->active == resize_down_id)
        if (int dy{pos.second - context->mouse_y}; current->h >= 40 || dy >= 0)
            current->h = current->h + dy;

    if (context->active == resize_right_down_id)
    {
        if (int dx{pos.first - context->mouse_x}; current->w >= 40 || dx >= 0)
            current->w = current->w + dx;
        if (int dy{pos.second - context->mouse_y}; current->h >= 40 || dy >= 0)
            current->h = current->h + dy;
    }

    // updating
    context->mouse_x = pos.first;
    context->mouse_y = pos.second;

    current->next_y = current->y + 25;

    // drawing
    draw_rect(win, {current->x, current->y, current->w, current->h}, {50, 50, 50, 50});   // body
    draw_rect(win, {current->x, current->y, current->w, 20}, window_cfg->titlebar_color); // title bar
    draw_text(win, name.data(), current->x + 16, current->y + 3);                         // title

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

    context->on_window = false;
    context->last_window->next_y = 0;
    context->last_window->menu_called = false;

    if (!context->mouse_down)
        context->active = 0;
    else
    {
        if (context->active == 0)
            context->active = -1;
    }
}

void BeginList(bool indexed) noexcept
{
    ++context->list_indent_level;
}

void EndList() noexcept
{
    if (context->list_indent_level <= 0)
        tnt::logger::debug("Line: {}\tCalling tnt::ImGui::EndList() without calling tnt::ImGui::BeginList() first!!");
    else
        --context->list_indent_level;
}

bool button(Window *win, std::string_view text) noexcept
{
    if (context->last_window->next_y + button_cfg->h >
            context->last_window->y + context->last_window->h ||
        context->last_window->w < button_cfg->w + 10)
        return false;
    std::string key{context->last_window->title};
    key.append(text);
    std::size_t id{im_hash(key)};
    button_cfg->w = static_cast<int>(text.size()) * 7 + 10;
    button_cfg->h = 10 + global_cfg->font_size;

    check_button(id, context->last_window->x + 10, context->last_window->next_y, button_cfg->w, button_cfg->h);

    SDL_Rect dst{context->last_window->x + 10, context->last_window->next_y, button_cfg->w, button_cfg->h};
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

    draw_text(win, text.data(), context->last_window->x + 15, context->last_window->next_y + 5, global_cfg->text_color, global_cfg->font_size);

    context->last_window->next_y = context->last_window->next_y + 30;

    if (context->hot == id && context->active == id)
        return true;
    return false;
}

bool slider_int(Window *win, std::size_t id, int x, int y, int min_, int max_,
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

bool slider_float(Window *win, std::size_t id, int x, int y, float min_, float max_,
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

bool hslider_int(Window *win, std::size_t id, int x, int y, int min_, int max_,
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

bool hslider_float(Window *win, std::size_t id, int x, int y, float min_, float max_,
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

int menu(Window *win, std::string_view *options, int size) noexcept
{
    if (context->last_window->next_y + global_cfg->font_size + 6 > context->last_window->y + context->last_window->h)
        return -1;

    if (context->last_window->menu_called)
    {
        tnt::logger::debug("Line: {}\tCalling tnt::ImGui::menu() for the second time inside the same window!!", __LINE__);
        return -1;
    }

    int ret{-1};
    for (int i{0}, lastX{context->last_window->x};
         i < size; ++i, lastX += menu_cfg->spacing + ((static_cast<int>(options[i - 1].size()) - 1) * 8))
    {
        std::size_t id{im_hash(options[i])};
        check_button(id, lastX, context->last_window->y + 20, static_cast<int>(options[i].size()) * 7 + 10, 10 + global_cfg->font_size);
        draw_text(win, options[i].data(), lastX + 10, context->last_window->y + 20);

        if (context->hot == id && context->active == id)
            ret = i;
    }

    context->last_window->next_y = context->last_window->next_y + global_cfg->font_size + 10;
    context->last_window->menu_called = true;
    return ret;
}

bool checkbox(Window *win, std::string_view text, bool *value) noexcept
{
    if (context->last_window->next_y + checkbox_cfg->length + 10 > context->last_window->y + context->last_window->h)
        return false;

    std::string key{context->last_window->title};
    key.append(text);
    std::size_t id{std::hash<std::string>{}(key)};

    SDL_Rect box{context->last_window->x + 10, context->last_window->y, checkbox_cfg->length, checkbox_cfg->length};

    if (on_rect(context->last_window->x + 10, context->last_window->y, checkbox_cfg->length, checkbox_cfg->length))
    {
        context->hot = id;
        if (context->active == 0 && context->mouse_down)
            context->active = id;
    }

    draw_rect(win, box, {130, 130, 130, 255});
    draw_text(win, text.data(), box.x + box.w + 10, context->last_window->next_y);
    context->last_window->next_y = context->last_window->next_y + box.h + 10;

    if (context->hot == id && context->active == id)
        *value = !(*value);

    if (*value)
    {
        SDL_RenderCopy(win->getRenderer(), checkbox_cfg->tex, nullptr, &box);
        return true;
    }

    return false;
}

void progress_bar(Window *win, std::string_view text, int min_, int max_,
                  int *value) noexcept
{
    if (context->last_window->w < progress_bar_cfg->w ||
        context->last_window->next_y + progress_bar_cfg->h >
            context->last_window->y + context->last_window->h)
        return;
    int xpos{(progress_bar_cfg->w * (*value - min_) / (max_ - min_))};
    draw_rect(win, {context->last_window->x + 10, context->last_window->next_y, progress_bar_cfg->w, progress_bar_cfg->h},
              progress_bar_cfg->idle_color);
    draw_text(win, text.data(),
              context->last_window->x + progress_bar_cfg->w + 20, context->last_window->next_y);
    draw_rect(win, {context->last_window->x + 12, context->last_window->next_y + 2, xpos, progress_bar_cfg->h - 4}, progress_bar_cfg->filled_color);
    context->last_window->next_y = context->last_window->next_y + progress_bar_cfg->h + 10;
}

void newline() noexcept
{
    if (context->last_window->next_y + global_cfg->font_size > context->last_window->y + context->last_window->h)
        return;
    context->last_window->next_y = context->last_window->next_y + global_cfg->font_size;
}

void text(Window *win, std::string_view text) noexcept
{
    if (context->last_window->w < global_cfg->font_size * text.size() ||
        context->last_window->next_y + global_cfg->font_size + 5 >
            context->last_window->y + context->last_window->h)
        return;
    draw_text(win, text.data(), context->last_window->x + 10, context->last_window->next_y);
    context->last_window->next_y = context->last_window->next_y + global_cfg->font_size + 5;
}

void list_item(Window *win, std::string_view text) noexcept
{
    if ((context->list_indent_level * 10) + (text.size() * global_cfg->font_size) <
            context->last_window->w ||
        context->last_window->next_y + global_cfg->font_size >
            context->last_window->y + context->last_window->h)
        return;
    int xpos{context->last_window->x + (context->list_indent_level * 10)};
    if (context->lists_text.find(text.data()) != context->lists_text.end())
        context->lists_text[text.data()] = load_text(win, text.data());
    draw_text(win, text.data(), xpos, context->last_window->next_y);
    context->last_window->next_y = context->last_window->next_y + global_cfg->font_size;
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

    tnt::ImGui::checkbox_cfg->tex = tnt::ImGui::load_image(win, "assets/tick.png");

    tnt::ImGui::make_context();
}

void tnt_imgui_close() noexcept
{
    SDL_DestroyTexture(tnt::ImGui::button_cfg->text);
    tnt::ImGui::button_cfg->text = nullptr;

    SDL_DestroyTexture(tnt::ImGui::checkbox_cfg->tex);
    tnt::ImGui::checkbox_cfg->tex = nullptr;

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