// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <map>

#include "core/Input.hpp"
#include "core/Window.hpp"
#include "imgui/ImGui.hpp"
#include "imgui/gui_config.hpp"
#include "utils/Logger.hpp"

namespace tnt::ImGui
{
    struct theme_t
    {
        int w, h;
        int font_size;

        int button_w, button_h;
        int slider_w, slider_h, slider_thumb_size;
        int hslider_h, hslider_thumb_size;
        int pbar_w, pbar_h;
        int checkbox_size;
        int menu_spacing;
        int section_h;

        SDL_Color bg;
        SDL_Color text_color;

        SDL_Color idle_color;   // for drawing non-active parts
        SDL_Color active_color; // for drawing active parts
        SDL_Color static_color; // for drawing non-movable parts

        std::string default_font;

        SDL_Texture *button_text;
        SDL_Texture *checkbox_tick;
        TTF_Font *font_data;

    } * theme{new theme_t{
            .w{0},
            .h{0},
            .font_size{14},

            .button_w{60},
            .button_h{40},

            .slider_w{20},
            .slider_h{100},
            .slider_thumb_size{18},
            .hslider_h{20},
            .hslider_thumb_size{18},

            .pbar_w{100},
            .pbar_h{20},
            .checkbox_size{20},
            .menu_spacing{20},
            .section_h{20},

            .text_color{.r{255}, .g{255}, .b{255}, .a{255}},
            .idle_color{.r{30}, .g{144}, .b{255}, .a{255}},
            .static_color{.r{130}, .g{130}, .b{130}, .a{255}},

            .default_font = std::string{SDL_GetBasePath()}.append("assets/Inconsolata.ttf")}};

    struct window_data
    {
        bool menu_called;
        bool collapsed;
        int x, y, w, h;
        int next_x, next_y; // next_x and next_y are used to arrange widgets on the window.
        int menu_index, menu_txt_size, last_menu_txt_size, menu_item_index;
        int context_menu_index;
        int list_indent_level, list_index, list_number;
        WindowFlags win_flags;
        std::string title;
    };

    struct context_t
    {
        bool on_window; // this is true if calling Begin() inside other Begin/End pair.
        bool mouse_down;
        int mouse_x, mouse_y;
        std::size_t active, hot, last_section;
        // std::string hash_prefix; // added to each widget's label when hashing
        std::string last_window;

        std::map<std::size_t, bool> sections; // cache to check if sections are collapsed or not
        std::map<std::string, SDL_Texture *> lists_text;
        std::map<std::string, window_data *> windows;
    } * context{new context_t{}};

#define TNT_IMGUI_RUNTIME_CONFIG

    ////////////
    // global //
    ////////////

    int get_window_w() noexcept { return theme->w; }
    int get_window_h() noexcept { return theme->h; }

    // TODO: load the other font texture here.
    void set_font(char const *name) noexcept
    {
        theme->default_font = name;
        if (theme->font_data != nullptr)
            TTF_CloseFont(theme->font_data);
        theme->font_data = TTF_OpenFont(name, theme->font_size);
    }

    /////////////////
    // local utils //
    /////////////////

    auto on_rect = [](int x, int y, int w, int h) -> bool {
        return ((context->mouse_x >= x) && (context->mouse_x < x + w) &&
                (context->mouse_y >= y) && (context->mouse_y < y + h));
    };

    auto load_text = [](Window const *win, char const *text,
                        SDL_Color color = theme->text_color) -> SDL_Texture * {
        SDL_Surface *temp{TTF_RenderText_Blended(theme->font_data, text, color)};
        if (temp == nullptr)
        {
            tnt::logger::debug("Couldn't load surface for text {} with size {}!!", text,
                               theme->font_size);
            return nullptr;
        }

        SDL_Texture *ret{SDL_CreateTextureFromSurface(win->getRenderer(), temp)};

        SDL_FreeSurface(temp);
        temp = nullptr;

        if (ret == nullptr)
        {
            tnt::logger::debug("Couldn't load font texture for text {} with size {}!!", text,
                               theme->font_size);
            return nullptr;
        }

        return ret;
    };

    auto load_image = [](Window const *win, char const *path) -> SDL_Texture * {
        return IMG_LoadTexture(win->getRenderer(), path);
    };

    auto draw_rect = [](Window const *win, SDL_Rect const &rect, SDL_Color const &color) -> void {
        SDL_RenderDrawRect(win->getRenderer(), &rect);
        SDL_SetRenderDrawColor(win->getRenderer(), color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(win->getRenderer(), &rect);
        SDL_SetRenderDrawColor(win->getRenderer(), theme->bg.r, theme->bg.g, theme->bg.b, theme->bg.a);
    };

    auto draw_line = [](Window const *win, int x1, int y1, int x2, int y2, SDL_Color const &color) -> void {
        SDL_SetRenderDrawColor(win->getRenderer(), color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(win->getRenderer(), x1, y1, x2, y2);
        SDL_SetRenderDrawColor(win->getRenderer(), theme->bg.r, theme->bg.g, theme->bg.b, theme->bg.a);
    };

    auto draw_text = [](Window const *win, char const *text, int x, int y,
                        SDL_Color color = theme->text_color,
                        int size = theme->font_size) -> void {
        SDL_Texture *cache{load_text(win, text, color)};
        int w{0}, h{0};
        SDL_QueryTexture(cache, nullptr, nullptr, &w, &h);
        SDL_Rect where{x, y, w * size / theme->font_size, h * size / theme->font_size};
        SDL_RenderCopy(win->getRenderer(), cache, nullptr, &where);
        SDL_DestroyTexture(cache);
        cache = nullptr;
    };

    auto has_flag = [](WindowFlags const &owner, WindowFlags const &test) -> bool {
        return ((owner & test) == test);
    };

    auto check_button = [](std::size_t id, int x, int y, int w, int h) -> void {
        if (on_rect(x, y, w, h))
        {
            context->hot = id;
            if (context->active == 0 && context->mouse_down) // left pressed
                context->active = id;
        }
    };

    // thx Wren
    // https://stackoverflow.com/a/11819477/9807802
    auto im_hash = [](std::string_view text) {
        std::size_t h{0};
        char const *str{text.data()};
        while (*str)
            h = h << 1 ^ *str++;
        return h;
    };

    auto get_window = [](std::string_view name) -> window_data * {
        if (context->windows.find(name.data()) != context->windows.end())
            return context->windows[name.data()];
        window_data *tmp{new window_data{.title = name.data()}};
        return tmp;
    };

    auto get_last_win = [] { return get_window(context->last_window); };

    auto hslider_basef = [](tnt::Window const *win, window_data const *tmp,
                            std::size_t const &id, int const &x,
                            int const &w, float const &min_,
                            float const &max_, float *value) -> bool {
        const int xpos{static_cast<int>(((w - theme->hslider_thumb_size) * (*value - min_)) /
                                        (max_ - min_))};
        const int offset{theme->hslider_h - theme->hslider_thumb_size};
        const SDL_Rect thumb{x + (offset / 2) + xpos, tmp->next_y + (offset / 2), theme->hslider_thumb_size,
                             theme->hslider_thumb_size};

        check_button(id, x, tmp->next_y, w, theme->hslider_h);

        draw_rect(win, {x, tmp->next_y, w + offset, theme->hslider_h}, theme->static_color);
        draw_rect(win, thumb, theme->idle_color);

        draw_text(win, std::to_string(*value).c_str(), x + w / 2 - 4, tmp->next_y);

        if (context->active == id)
        {
            int mousePos{context->mouse_x - x - (offset / 2)};
            if (mousePos < 0)
                mousePos = 0;
            if (mousePos > w - 1)
                mousePos = w - 1;

            if (float v{static_cast<float>(mousePos * (max_ - min_) / (w - 1)) + min_};
                v != (*value))
            {
                *value = (float)((int)(v * 10.f)) * .1f;
                return true;
            }
        }

        return false;
    };

    auto hslider_basei = [](tnt::Window const *win, window_data const *tmp,
                            std::size_t const &id, int const &x,
                            int const &w, int const &min_,
                            int const &max_, int *value) -> bool {
        const int xpos{(w - theme->hslider_thumb_size) * (*value - min_) /
                       (max_ - min_)};
        const int offset{theme->hslider_h - theme->hslider_thumb_size};
        const SDL_Rect thumb{x + (offset / 2) + xpos, tmp->next_y + (offset / 2), theme->hslider_thumb_size,
                             theme->hslider_thumb_size};

        check_button(id, x, tmp->next_y, w, theme->hslider_h);

        draw_rect(win, {x, tmp->next_y, w + offset, theme->hslider_h}, theme->static_color);
        draw_rect(win, thumb, theme->idle_color);

        draw_text(win, std::to_string(*value).c_str(), x + w / 2 - 4, tmp->next_y);

        if (context->active == id)
        {
            int mousePos{context->mouse_x - x - (offset / 2)};
            if (mousePos < 0)
                mousePos = 0;
            if (mousePos > w - 1)
                mousePos = w - 1;

            if (int v{mousePos * (max_ - min_) / (w - 1) + min_}; v != (*value))
            {
                *value = v;
                return true;
            }
        }

        return false;
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
        tnt::input::updateCurrent();
        tnt::input::updatePrevious();
        context->mouse_down = input::mouseButtonDown(0);
    }

    void destroy_context() noexcept
    {
        context->mouse_down = false;
        context->mouse_x = 0;
        context->mouse_y = 0;
        context->active = 0;
        context->hot = 0;

        for (auto const &it : context->lists_text)
            if (it.second != nullptr)
                SDL_DestroyTexture(it.second);

        for (auto const &it : context->windows)
            if (it.second != nullptr)
                delete it.second;

        delete context;
        context = nullptr;
    }

    /////////////
    // widgets //
    /////////////

    bool Begin(Window const *win, std::string_view name, int x_, int y_, WindowFlags flags) noexcept
    {
        window_data *tmp{get_window(name.data())};

        if (context->windows.find(name.data()) == context->windows.end())
        {
            if (context->last_window == name.data())
            {
                tnt::logger::debug("Calling Begin() twice for the same window title!!");
                return false;
            }

            tmp->menu_called = false;
            tmp->collapsed = false;
            tmp->x = x_;
            tmp->y = y_;
            tmp->w = 400;
            tmp->h = 300;
            tmp->win_flags = flags;
            tmp->title = name.data();

            context->windows[name.data()] = tmp;
        }

        context->on_window = true;
        context->last_window = name;

        context->hot = 0;
        tmp->list_indent_level = 0;
        tmp->list_index = 0;
        tmp->next_x = 10 + tmp->x;

        const std::pair pos{input::mousePosition()};
        const std::size_t id{im_hash(name)};

        if (has_flag(tmp->win_flags, WindowFlags::Collapsible))
        {
            const std::size_t collapse_id{10 + id};

            check_button(collapse_id, tmp->x + 5, tmp->y + 3, 10, 10);

            if (context->active == collapse_id)
                tmp->collapsed = !tmp->collapsed;
        }

        // check for moving
        if (has_flag(tmp->win_flags, WindowFlags::Movable))
        {
            check_button(id, tmp->x, tmp->y, tmp->w, 20);

            if (context->active == id)
            {
                tmp->x = tmp->x + pos.first - context->mouse_x;
                tmp->y = tmp->y + pos.second - context->mouse_y;
            }
        }

        if (!tmp->collapsed)
            if (has_flag(tmp->win_flags, WindowFlags::Resizable))
            {
                const std::size_t minW{name.size() * 7 + 32};

                // TODO: make sure these *_id don't match the id of any other widget.
                const std::size_t resize_left_id{1 + id};
                const std::size_t resize_right_id{2 + id};
                const std::size_t resize_down_id{3 + id};

                const std::size_t resize_right_down_id{4 + id};
                const std::size_t resize_left_down_id{5 + id};

                // check for resizing
                check_button(resize_right_id, tmp->x + tmp->w - 10,
                             tmp->y + 20, 10, tmp->h - 10);
                check_button(resize_down_id, tmp->x + 10,
                             tmp->y + tmp->h - 10, tmp->w - 20, 10);
                check_button(resize_right_down_id, tmp->x + tmp->w - 10,
                             tmp->y + tmp->h - 10, 10, 10);

                if (context->active == resize_right_down_id)
                {
                    if (int dx{pos.first - context->mouse_x}; tmp->w >= minW || dx >= 0)
                        tmp->w = tmp->w + dx;
                    if (int dy{pos.second - context->mouse_y}; tmp->h >= 40 || dy >= 0)
                        tmp->h = tmp->h + dy;
                }

                if (context->active == resize_right_id)
                    if (int dx{pos.first - context->mouse_x}; tmp->w >= minW || dx >= 0) // bigger than double of the height of the title bar
                        tmp->w = tmp->w + dx;

                if (context->active == resize_down_id)
                    if (int dy{pos.second - context->mouse_y}; tmp->h >= 40 || dy >= 0)
                        tmp->h = tmp->h + dy;
            }

        context->mouse_x = pos.first;
        context->mouse_y = pos.second;

        tmp->next_y = tmp->y + 25;

        // drawing
        if (has_flag(tmp->win_flags, WindowFlags::OpaqueBackground))
        {
            if (!tmp->collapsed)
                draw_rect(win, {tmp->x, tmp->y, tmp->w, tmp->h}, {50, 50, 50, 255}); // body
            if (has_flag(tmp->win_flags, WindowFlags::WithTitleBar))
                draw_rect(win, {tmp->x, tmp->y, tmp->w, 20}, theme->idle_color); // title bar
        }

        if (has_flag(tmp->win_flags, WindowFlags::Collapsible))
        {
            draw_rect(win, {tmp->x + 5, tmp->y + 8, 10, 4}, {255, 255, 255});

            if (tmp->collapsed)
                draw_rect(win, {tmp->x + 8, tmp->y + 5, 4, 10}, {255, 255, 255});
        }

        if (has_flag(tmp->win_flags, WindowFlags::WithTitleBar))
            draw_text(win, name.data(), tmp->x + 25, tmp->y + 3); // title

        return true;
    }

    void End() noexcept
    {
        if (!context->on_window)
        {
            tnt::logger::debug("Calling tnt::ImGui::End() without "
                               "calling tnt::ImGui::Begin() before!!");
            return;
        }

        context->on_window = false;
        window_data *last{get_last_win()};
        last->next_y = 0;
        last->menu_called = false;

        if (!context->mouse_down)
            context->active = 0;
        else
        {
            if (context->active == 0)
                context->active = -1;
        }
    }

    bool BeginSection(Window const *win, std::string_view text) noexcept
    {
        window_data *tmp{get_last_win()};
        const int w{tmp->w - static_cast<int>(10)}; // 10 padding to left, 10 to right

        // 40 = 5 + 5 left/right padding for +/-, 10 right padding for text, 10 +/- size.
        if (tmp->collapsed || tmp->next_y + theme->section_h > tmp->y + tmp->h ||
            tmp->w < static_cast<int>(text.size() * 7 + 40))
            return false;

        const std::size_t id{im_hash(tmp->title + text.data())};
        context->last_section = id;

        check_button(id, tmp->next_x + 5, tmp->next_y + 5, 10, 10);

        if (context->active == id)
            context->sections[id] = !context->sections[id];

        draw_rect(win, {tmp->next_x - 5, tmp->next_y, w, theme->section_h}, theme->idle_color);
        draw_rect(win, {tmp->next_x + 5, tmp->next_y + 8, 10, 4}, {255, 255, 255});

        if (!context->sections[id])
            draw_rect(win, {tmp->next_x + 8, tmp->next_y + 5, 4, 10}, {255, 255, 255});
        else
            tmp->next_x = tmp->next_x + 10;

        draw_text(win, text.data(), tmp->next_x + 20, tmp->next_y + 1);
        tmp->next_y = tmp->next_y + 30;

        return context->sections[id];
    }

    void EndSection() noexcept
    {
        window_data *tmp{get_last_win()};

        if (context->sections[context->last_section] && !tmp->collapsed)
            tmp->next_x = tmp->next_x - 10;
    }

    void BeginList(bool indexed) noexcept
    {
        window_data *tmp{get_last_win()};
        ++tmp->list_indent_level;
    }

    void EndList() noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->list_indent_level <= 0)
            tnt::logger::debug("Line: {}\tCalling tnt::ImGui::EndList() without calling tnt::ImGui::BeginList() first!!");
        else
            --tmp->list_indent_level;
    }

    void BeginMenuBar() noexcept
    {
        if (window_data * tmp{get_last_win()}; !(tmp->menu_called || tmp->collapsed))
        {
            tmp->menu_index = 0;
            tmp->menu_txt_size = 0;
            tmp->last_menu_txt_size = 0;
            tmp->menu_item_index = 0;
            tmp->context_menu_index = 0;
        }
    }

    void EndMenuBar() noexcept
    {
        if (window_data * tmp{get_last_win()}; tmp->collapsed)
        {
            tmp->menu_called = true;
            tmp->next_y = tmp->next_y + theme->font_size + 10;
        }
    }

    bool button(Window const *win, std::string_view text) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->next_y + theme->button_h > tmp->y + tmp->h ||
            tmp->w < theme->button_w + 10)
            return false;

        std::string key{tmp->title};
        key.append(text);
        const std::size_t id{im_hash(key)};

        theme->button_w = static_cast<int>(text.size()) * 7 + 10;
        theme->button_h = 10 + theme->font_size;

        check_button(id, tmp->next_x, tmp->next_y, theme->button_w, theme->button_h);

        const SDL_Rect dst{tmp->next_x, tmp->next_y, theme->button_w, theme->button_h};
        SDL_Color widgetColor;

        if (context->hot == id && context->active == id)
            widgetColor = theme->active_color;
        else
            widgetColor = theme->idle_color;

        draw_rect(win, dst, widgetColor);
        draw_text(win, text.data(), tmp->next_x + 5, tmp->next_y + 5, theme->text_color, theme->font_size);

        tmp->next_y = tmp->next_y + 30;

        if (context->hot == id && context->active == id)
            return true;
        return false;
    }

    bool slider_int(Window const *win, int min_, int max_,
                    int *value) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->next_y + theme->slider_h > tmp->y + tmp->h ||
            tmp->w < theme->slider_w)
            return false;

        const std::size_t id{im_hash(tmp->title + std::to_string(*value))};

        const int ypos{((theme->slider_h - theme->slider_thumb_size) * (*value - min_)) / (max_ - min_)};
        const int offset{theme->slider_w - theme->slider_thumb_size};

        const SDL_Rect thumb{tmp->next_x + (offset / 2), tmp->next_y + (offset / 2) + ypos, theme->slider_thumb_size,
                             theme->slider_thumb_size};

        check_button(id, tmp->next_x, tmp->next_y, theme->slider_w, theme->slider_h);

        draw_rect(win, {tmp->next_x, tmp->next_y, theme->slider_w, theme->slider_h + offset}, theme->static_color);
        draw_rect(win, thumb, theme->idle_color);

        bool ret{false};

        if (context->active == id)
        {
            int mousePos{context->mouse_y - (tmp->next_y + (offset / 2))};
            if (mousePos < 0)
                mousePos = 0;
            if (mousePos > (theme->slider_h - 1))
                mousePos = (theme->slider_h - 1);
            if (int v{min_ + (mousePos * (max_ - min_)) / (theme->slider_h - 1)}; v != *value)
            {
                *value = v;
                ret = true;
            }
        }

        tmp->next_y = tmp->next_y + theme->slider_h + 5;

        return ret;
    }

    bool slider_float(Window const *win, float min_, float max_,
                      float *value) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->next_y + theme->slider_h > tmp->y + tmp->h ||
            tmp->w < theme->slider_w)
            return false;

        const std::size_t id{im_hash(tmp->title + std::to_string(*value))};
        const int ypos{static_cast<int>(((theme->slider_h - theme->slider_thumb_size) * (*value - min_)) /
                                        (max_ - min_))};
        const int offset{theme->slider_w - theme->slider_thumb_size};

        if (on_rect(tmp->next_x, tmp->next_y, theme->slider_w, theme->slider_h))
        {
            context->hot = id;
            if (context->active == 0 && context->mouse_down)
                context->active = id;
        }

        draw_rect(win, {tmp->next_x, tmp->next_y, theme->slider_w, theme->slider_h + offset}, theme->static_color);

        const SDL_Rect thumb{tmp->next_x + (offset / 2), tmp->next_y + (offset / 2) + ypos, theme->slider_thumb_size,
                             theme->slider_thumb_size};

        draw_rect(win, thumb, theme->idle_color);

        bool ret{false};

        if (context->active == id)
        {
            int mousePos{context->mouse_y - (tmp->next_y + (offset / 2))};
            if (mousePos < 0)
                mousePos = 0;
            if (mousePos > (theme->slider_h - 1))
                mousePos = (theme->slider_h - 1);
            if (float v{static_cast<float>(min_ + (mousePos * (max_ - min_)) / (theme->slider_h - 1))}; v != *value)
            {
                *value = v;
                ret = true;
            }
        }

        tmp->next_y = tmp->next_y + theme->slider_h + 5;

        return ret;
    }

    bool hslider_int(Window const *win, std::string_view text, int min_, int max_,
                     int *value) noexcept
    {
        window_data *tmp{get_last_win()};

        // 40 = 10 for left padding, 20 for distance between text and slider, 10 for right padding.
        const int w{tmp->w - static_cast<int>(7 * text.size() + 40)};

        // 50 + 10 + 20, 50 -> slider minimal width, 10 -> padding from the left of the window, 20 -> distance between the slider and it's text
        if (tmp->collapsed || tmp->next_y + theme->hslider_h > tmp->y + tmp->h ||
            tmp->w < static_cast<int>(80 + 7 * text.size()))
            return false;

        const std::size_t id{im_hash(tmp->title + text.data())};
        const bool ret{hslider_basei(win, tmp, id, tmp->next_x, w, min_, max_, value)};

        draw_text(win, text.data(), tmp->next_x + w + 20, tmp->next_y);

        tmp->next_y = tmp->next_y + theme->hslider_h + 5;

        return ret;
    } // namespace tnt::ImGui

    bool hslider_float(Window const *win, std::string_view text, float min_, float max_,
                       float *value) noexcept
    {
        window_data *tmp{get_last_win()};

        // 40 = 10 for left padding, 20 for distance between text and slider, 10 for right padding.
        const int w{tmp->w - static_cast<int>(7 * text.size() + 40)};

        // 50 + 10 + 20, 50 -> slider minimal width, 10 -> padding from the left of the window, 20 -> distance between the slider and it's text
        if (tmp->collapsed || tmp->next_y + theme->hslider_h > tmp->y + tmp->h ||
            tmp->w < static_cast<int>(80 + 7 * text.size()))
            return false;

        const std::size_t id{im_hash(tmp->title + text.data())};
        const bool ret{hslider_basef(win, tmp, id, tmp->next_x, w, min_, max_, value)};

        draw_text(win, text.data(), tmp->next_x + w + 20, tmp->next_y);

        tmp->next_y = tmp->next_y + theme->hslider_h + 5;

        return ret;
    }

    bool hslider_int2(Window const *win, std::string_view text,
                      int min_, int max_, int *value1, int *value2) noexcept
    {
        window_data *tmp{get_last_win()};

        // 50 = 10 for left padding, 20 for distance between text and slider,
        // 10 for right padding, 10 for distance between sliders.
        const int w{(tmp->w - static_cast<int>(7 * text.size())) / 2 - 25};

        // 50 + 10 + 20, 50 -> slider minimal width, 10 -> padding from the left of the window, 20 -> distance between the slider and it's text
        if (tmp->collapsed || tmp->next_y + theme->hslider_h > tmp->y + tmp->h ||
            tmp->w < static_cast<int>(80 + 7 * text.size()))
            return false;

        const std::size_t id1{im_hash(tmp->title + text.data() + "x")};
        const std::size_t id2{im_hash(tmp->title + text.data() + "y")};

        const bool ret1{hslider_basei(win, tmp, id1, tmp->next_x, w, min_, max_, value1)};
        const bool ret2{hslider_basei(win, tmp, id2, tmp->next_x + w + 10, w, min_, max_, value2)};

        draw_text(win, text.data(), tmp->next_x + 2 * w + 30, tmp->next_y);

        tmp->next_y = tmp->next_y + theme->hslider_h + 5;

        return (ret1 || ret2);
    }

    bool hslider_float2(Window const *win, std::string_view text,
                        float min_, float max_, float *value1, float *value2) noexcept
    {
        window_data *tmp{get_last_win()};

        // 50 = 10 for left padding, 20 for distance between text and slider,
        // 10 for right padding, 10 for distance between sliders.
        const int w{(tmp->w - static_cast<int>(7 * text.size())) / 2 - 25};

        // 50 + 10 + 20, 50 -> slider minimal width, 10 -> padding from the left of the window, 20 -> distance between the slider and it's text
        if (tmp->collapsed || tmp->next_y + theme->hslider_h > tmp->y + tmp->h ||
            tmp->w < static_cast<int>(80 + 7 * text.size()))
            return false;

        const std::size_t id1{im_hash(tmp->title + text.data() + "x")};
        const std::size_t id2{im_hash(tmp->title + text.data() + "y")};

        const bool ret1{hslider_basef(win, tmp, id1, tmp->next_x, w, min_, max_, value1)};
        const bool ret2{hslider_basef(win, tmp, id2, tmp->next_x + w + 10, w, min_, max_, value2)};

        draw_text(win, text.data(), tmp->next_x + 2 * w + 30, tmp->next_y);

        tmp->next_y = tmp->next_y + theme->hslider_h + 5;

        return (ret1 || ret2);
    }

    bool hslider_vec(Window const *win, std::string_view text,
                     float min1, float max1, float min2, float max2,
                     tnt::Vector *value) noexcept
    {
        window_data *tmp{get_last_win()};

        // 50 = 10 for left padding, 20 for distance between text and slider,
        // 10 for right padding, 10 for distance between sliders.
        const int w{(tmp->w - static_cast<int>(7 * text.size())) / 2 - 25};

        // 50 + 10 + 20, 50 -> slider minimal width, 10 -> padding from the left of the window, 20 -> distance between the slider and it's text
        if (tmp->collapsed || tmp->next_y + theme->hslider_h > tmp->y + tmp->h ||
            tmp->w < static_cast<int>(80 + 7 * text.size()))
            return false;

        auto [x_, y_]{*value};

        const std::size_t id1{im_hash(tmp->title + text.data() + "x")};
        const std::size_t id2{im_hash(tmp->title + text.data() + "y")};

        const bool ret1{hslider_basef(win, tmp, id1, tmp->next_x, w, min1, max1, &x_)};
        const bool ret2{hslider_basef(win, tmp, id2, tmp->next_x + w + 10, w, min2, max2, &y_)};

        if (x_ != value->x)
            value->x = x_;
        if (y_ != value->y)
            value->y = y_;

        draw_text(win, text.data(), tmp->next_x + 2 * w + 30, tmp->next_y);

        tmp->next_y = tmp->next_y + theme->hslider_h + 5;

        return (ret1 || ret2);
    }

    bool menu_button(Window const *win, std::string_view text) noexcept
    {
        window_data *tmp{get_last_win()};

        const int x{tmp->x + tmp->menu_index * theme->menu_spacing + 7 * tmp->menu_txt_size};

        if (tmp->collapsed || x + static_cast<int>(text.size() + 1) * 7 > tmp->x + tmp->w)
            return false;

        const std::size_t id{im_hash(tmp->title + text.data())};

        check_button(
            id, x, tmp->y + 20,
            static_cast<int>(text.size()) * 7, theme->font_size + 6);
        draw_text(win, text.data(), x + 10, tmp->y + 20);

        tmp->menu_index = tmp->menu_index + 1;
        tmp->last_menu_txt_size = tmp->menu_txt_size;
        tmp->menu_txt_size = tmp->menu_txt_size + static_cast<int>(text.size());

        if (context->hot == id && context->active == id)
            return true;
        return false;
    }

    bool menu_item(Window const *win, std::string_view text) noexcept
    {
        window_data *tmp{get_last_win()};

        const int x{tmp->x + (tmp->menu_index - 1) * theme->menu_spacing + 7 * tmp->last_menu_txt_size};

        // 100 = 30 -> padding from left and right + 70 -> size of 10 letters (with the current font)
        if (tmp->collapsed || x + 100 > tmp->x + tmp->w)
            return false;

        const std::size_t id{im_hash(tmp->title + text.data())};

        check_button(id, x, tmp->next_y, 100, theme->font_size + 10);

        SDL_Color color{0, 0, 0, 200};

        if (context->hot == id && context->active == id)
            color = SDL_Color{50, 50, 50, 200};

        draw_rect(win, {x, tmp->next_y + theme->font_size, 100, theme->font_size + 10}, color);
        draw_text(win, text.data(), x + 15, tmp->next_y + 5 + theme->font_size);

        tmp->context_menu_index = tmp->context_menu_index + 1;

        return false;
    }

    bool checkbox(Window const *win, std::string_view text, bool *value) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->next_y + theme->checkbox_size + 10 > tmp->y + tmp->h)
            return false;

        const std::size_t id{im_hash(tmp->title + text.data())};

        check_button(id, tmp->next_x, tmp->next_y, theme->checkbox_size, theme->checkbox_size);

        draw_rect(win, {tmp->next_x, tmp->next_y, theme->checkbox_size, theme->checkbox_size},
                  {130, 130, 130, 255});
        draw_text(win, text.data(), tmp->next_x + theme->checkbox_size + 10, tmp->next_y);
        tmp->next_y = tmp->next_y + theme->checkbox_size + 10;

        if (context->hot == id && context->active == id)
            *value = !(*value);

        if (*value)
        {
            const SDL_Rect box{tmp->next_x, tmp->next_y, theme->checkbox_size, theme->checkbox_size};
            SDL_RenderCopy(win->getRenderer(), theme->checkbox_tick, nullptr, &box);
            return true;
        }

        return false;
    }

    void progress_bar(Window const *win, std::string_view text, int min_, int max_,
                      int *value) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->w < theme->pbar_w ||
            tmp->next_y + theme->pbar_h > tmp->y + tmp->h)
            return;
        const int xpos{theme->pbar_w * (*value - min_) / (max_ - min_)};

        draw_rect(win, {tmp->x + 10, tmp->next_y, theme->pbar_w, theme->pbar_h},
                  theme->idle_color);
        draw_text(win, text.data(), tmp->x + theme->pbar_w + 20, tmp->next_y);
        draw_rect(win, {tmp->x + 12, tmp->next_y + 2, xpos, theme->pbar_h - 4}, theme->active_color);

        tmp->next_y = tmp->next_y + theme->pbar_h + 10;
    }

    void newline() noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->next_y + theme->font_size > tmp->y + tmp->h)
            return;
        tmp->next_y = tmp->next_y + theme->font_size;
    }

    void text(Window const *win, std::string_view text) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->w < 10 + 7 * text.size() ||
            tmp->next_y + theme->font_size + 5 > tmp->y + tmp->h)
            return;
        draw_text(win, text.data(), tmp->next_x, tmp->next_y);
        tmp->next_y = tmp->next_y + theme->font_size + 5;
    }

    void colored_text(Window const *win, std::string_view text,
                      unsigned char r, unsigned char g,
                      unsigned char b, unsigned char a) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->w < 10 + 7 * text.size() ||
            tmp->next_y + theme->font_size + 5 > tmp->y + tmp->h)
            return;
        draw_text(win, text.data(), tmp->next_x, tmp->next_y, {r, g, b, a});
        tmp->next_y = tmp->next_y + theme->font_size + 5;
    }

    void list_item(Window const *win, std::string_view text) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed ||
            tmp->list_indent_level * 10 + text.size() * theme->font_size < tmp->w ||
            tmp->next_y + theme->font_size > tmp->y + tmp->h)
            return;
        const int xpos{tmp->x + tmp->list_indent_level * 10};
        if (context->lists_text.find(text.data()) != context->lists_text.cend())
            context->lists_text[text.data()] = load_text(win, text.data());
        draw_text(win, text.data(), xpos, tmp->next_y);
        tmp->next_y = tmp->next_y + theme->font_size;
    }
} // namespace tnt::ImGui

/////////////////
// tnt_imgui_* //
/////////////////

void tnt_imgui_init(tnt::Window const *win) noexcept
{
    int w{0}, h{0};
    SDL_GetWindowSize((SDL_Window *)win, &w, &h);
    if (tnt::ImGui::theme->w == 0)
        tnt::ImGui::theme->w = w;
    if (tnt::ImGui::theme->h == 0)
        tnt::ImGui::theme->h = h;

    tnt::ImGui::theme->font_data = TTF_OpenFont(tnt::ImGui::theme->default_font.data(),
                                                tnt::ImGui::theme->font_size);
    tnt::ImGui::theme->bg = win->getClearColor();

    tnt::ImGui::theme->checkbox_tick = tnt::ImGui::load_image(win, "assets/tick.png");

    tnt::ImGui::make_context();
}

void tnt_imgui_close() noexcept
{
    SDL_DestroyTexture(tnt::ImGui::theme->button_text);
    tnt::ImGui::theme->button_text = nullptr;

    SDL_DestroyTexture(tnt::ImGui::theme->checkbox_tick);
    tnt::ImGui::theme->checkbox_tick = nullptr;

    TTF_CloseFont(tnt::ImGui::theme->font_data);
    tnt::ImGui::theme->font_data = nullptr;

    delete tnt::ImGui::theme;
    tnt::ImGui::theme = nullptr;

    tnt::ImGui::destroy_context();
}