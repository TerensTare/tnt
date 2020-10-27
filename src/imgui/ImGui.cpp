// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <map>
#include <charconv>

#include "core/Input.hpp"
#include "imgui/ImGui.hpp"
#include "imgui/gui_config.hpp"
#include "utils/Logger.hpp"

// TODO: use std::unordered_map

namespace tnt::ImGui
{
    inline static struct theme_t final
    {
        inline ~theme_t() noexcept
        {
            if (init)
            {
                SDL_DestroyTexture(tnt::ImGui::theme.button_text);
                SDL_DestroyTexture(tnt::ImGui::theme.checkbox_tick);

                TTF_CloseFont(tnt::ImGui::theme.font_data);
            }
        }

        bool init{false};

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

        SDL_Texture *button_text;
        SDL_Texture *checkbox_tick;
        TTF_Font *font_data;

        std::string default_font;
    } theme{
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
        .active_color{.r{0}, .g{0}, .b{0}, .a{255}},
        .static_color{.r{130}, .g{130}, .b{130}, .a{255}},

        .default_font{std::string{SDL_GetBasePath()}.append("assets/Inconsolata.ttf")}};

    struct window_data final
    {
        bool menu_called;
        bool collapsed;
        WindowFlags win_flags;
        int x, y, w, h;
        int next_x, next_y; // next_x and next_y are used to arrange widgets on the window.
        int menu_index, menu_txt_size, last_menu_txt_size, menu_item_index;
        int context_menu_index;
        int list_indent_level, list_index, list_number;
        std::string title;
    };

    struct context_t final
    {
        inline ~context_t() noexcept
        {
            for (auto const &it : context.lists_text)
                if (it.second)
                    SDL_DestroyTexture(it.second);

            for (auto const &it : context.windows)
                if (it.second)
                    delete it.second;
        }

        bool on_window; // this is true if calling Begin() inside other Begin/End pair.
        bool mouse_down;
        int mouse_x, mouse_y;
        std::size_t active, hot, last_section;
        // std::string hash_prefix; // added to each widget's label when hashing
        std::string last_window;

        std::map<std::size_t, bool> sections; // cache to check if sections are collapsed or not
        std::map<std::string, SDL_Texture *, std::less<>> lists_text;
        std::map<std::string, window_data *, std::less<>> windows;
    } context;

#define TNT_IMGUI_RUNTIME_CONFIG

    ////////////
    // global //
    ////////////

    int get_window_w() noexcept { return theme.w; }
    int get_window_h() noexcept { return theme.h; }

    // TODO: load the other font texture here.
    void set_font(char const *name) noexcept
    {
        theme.default_font = name;
        if (theme.font_data != nullptr)
        {
            TTF_CloseFont(theme.font_data);
            theme.font_data = nullptr;
        }
        theme.font_data = TTF_OpenFont(name, theme.font_size);
    }

    /////////////////
    // local utils //
    /////////////////

    inline auto on_rect = [](int x, int y, int w, int h) noexcept -> bool {
        return ((context.mouse_x >= x) && (context.mouse_x < x + w) &&
                (context.mouse_y >= y) && (context.mouse_y < y + h));
    };

    inline auto load_text = [](Window const &win, char const *text,
                               SDL_Color color = theme.text_color) noexcept -> SDL_Texture * {
        SDL_Surface *temp{TTF_RenderText_Blended(theme.font_data, text, color)};
        if (temp == nullptr)
        {
            tnt::logger::debug("Couldn't load surface for text {} with size {}!!", text,
                               theme.font_size);
            return nullptr;
        }

        SDL_Texture *ret{SDL_CreateTextureFromSurface(win.getRenderer(), temp)};

        SDL_FreeSurface(temp);
        temp = nullptr;

        if (ret == nullptr)
        {
            tnt::logger::debug("Couldn't load font texture for text {} with size {}!!", text,
                               theme.font_size);
            return nullptr;
        }

        return ret;
    };

    inline auto load_image = [](Window const &win, char const *path) noexcept -> SDL_Texture * {
        return IMG_LoadTexture(win.getRenderer(), path);
    };

    inline auto draw_rect = [](Window const &win, SDL_Rect const &rect, SDL_Color const &color) noexcept -> void {
        SDL_SetRenderDrawColor(win.getRenderer(), color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(win.getRenderer(), &rect);
        SDL_SetRenderDrawColor(win.getRenderer(), theme.bg.r, theme.bg.g, theme.bg.b, theme.bg.a);
    };

    inline auto draw_line = [](Window const &win, int x1, int y1, int x2, int y2, SDL_Color const &color) noexcept -> void {
        SDL_SetRenderDrawColor(win.getRenderer(), color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(win.getRenderer(), x1, y1, x2, y2);
        SDL_SetRenderDrawColor(win.getRenderer(), theme.bg.r, theme.bg.g, theme.bg.b, theme.bg.a);
    };

    inline auto draw_text = [](Window const &win, char const *text, int x, int y,
                               SDL_Color color = theme.text_color,
                               int size = theme.font_size) noexcept -> void {
        SDL_Texture *cache{load_text(win, text, color)};
        int w{0}, h{0};
        SDL_QueryTexture(cache, nullptr, nullptr, &w, &h);
        SDL_Rect where{x, y, w * size / theme.font_size, h * size / theme.font_size};
        SDL_RenderCopy(win.getRenderer(), cache, nullptr, &where);
        SDL_DestroyTexture(cache);
        cache = nullptr;
    };

    inline auto has_flag = [](WindowFlags const &owner, WindowFlags const &test) noexcept -> bool {
        return ((owner & test) == test);
    };

    inline auto check_button = [](std::size_t id, int x, int y, int w, int h) noexcept -> void {
        if (on_rect(x, y, w, h))
        {
            context.hot = id;
            if (context.active == 0 && context.mouse_down) // left mouse button pressed
                context.active = id;
        }
    };

    inline auto update_context = []() noexcept -> void {
        tnt::input::Update();
        context.mouse_down = input::mouseButtonDown(0);
    };

    // thx Wren
    // https://stackoverflow.com/a/11819477/9807802
    auto im_hash = [](std::string_view text) {
        std::size_t h{0};
        for (char const *str{text.data()}; *str; ++str)
            h = h << 1 ^ *str;
        return h;
    };

    auto get_window = [](std::string_view name) -> window_data * {
        if (context.windows.contains(name.data()))
            return context.windows[name.data()];
        window_data *tmp{new window_data{.title{name.data()}}};
        return tmp;
    };

    auto get_last_win = [] { return get_window(context.last_window); };

    auto hslider_basef = [](tnt::Window const &win, window_data const *tmp,
                            std::size_t const &id, int const &x,
                            int const &w, float const &min_,
                            float const &max_, float *value) -> bool {
        const int xpos{static_cast<int>(((w - theme.hslider_thumb_size) * (*value - min_)) /
                                        (max_ - min_))};
        const int offset{theme.hslider_h - theme.hslider_thumb_size};
        const SDL_Rect thumb{x + (offset / 2) + xpos, tmp->next_y + (offset / 2), theme.hslider_thumb_size,
                             theme.hslider_thumb_size};

        check_button(id, x, tmp->next_y, w, theme.hslider_h);

        draw_rect(win, {x, tmp->next_y, w + offset, theme.hslider_h}, theme.static_color);
        draw_rect(win, thumb, theme.idle_color);

        {
            char temp[10]; // only 10 digits, use them wisely
            if (auto const &[ptr, ec]{std::to_chars(temp, temp + std::size(temp), *value)};
                ec == std::errc{})
                draw_text(win, temp, x + w / 2 - 4, tmp->next_y);
        }

        if (context.active == id)
        {
            int mousePos{context.mouse_x - x - (offset / 2)};
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

    auto hslider_basei = [](tnt::Window const &win, window_data const *tmp,
                            std::size_t const &id, int const &x,
                            int const &w, int const &min_,
                            int const &max_, int *value) -> bool {
        const int &xpos{(w - theme.hslider_thumb_size) * (*value - min_) /
                        (max_ - min_)};
        const int &offset{theme.hslider_h - theme.hslider_thumb_size};
        const SDL_Rect &thumb{x + (offset / 2) + xpos, tmp->next_y + (offset / 2), theme.hslider_thumb_size,
                              theme.hslider_thumb_size};

        check_button(id, x, tmp->next_y, w, theme.hslider_h);

        draw_rect(win, {x, tmp->next_y, w + offset, theme.hslider_h}, theme.static_color);
        draw_rect(win, thumb, theme.idle_color);

        {
            char temp[10]; // an integer can hold up to 10 digits
            if (auto const &[ptr, ec]{std::to_chars(temp, temp + std::size(temp), *value)};
                ec == std::errc{})
                draw_text(win, temp, x + w / 2 - 4, tmp->next_y);
        }

        if (context.active == id)
        {
            int mousePos{context.mouse_x - x - (offset / 2)};
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
        std::pair const pos{input::mousePosition()};

        context.on_window = false;
        context.mouse_down = input::mouseButtonDown(0);
        context.mouse_x = pos.first;
        context.mouse_y = pos.second;
        context.active = 0;
        context.hot = 0;
    }

    /////////////
    // widgets //
    /////////////

    bool Begin(Window const &win, std::string_view name, int x_, int y_, WindowFlags flags) noexcept
    {
        window_data *tmp{get_window(name.data())};

        if (context.windows.find(name.data()) == context.windows.cend())
        {
            if (context.last_window == name.data())
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

            context.windows[name.data()] = tmp;
        }

        context.on_window = true;
        context.last_window = name;

        context.hot = 0;

        tmp->list_indent_level = 0;
        tmp->list_index = 0;
        tmp->next_x = 10 + tmp->x;

        const std::pair<int, int> &pos{input::mousePosition()};
        const std::size_t &id{im_hash(name)};

        if (has_flag(tmp->win_flags, WindowFlags::Collapsible))
        {
            const std::size_t collapse_id{10 + id};

            check_button(collapse_id, tmp->x + 5, tmp->y + 3, 10, 10);

            if (context.active == collapse_id)
                tmp->collapsed = !tmp->collapsed;
        }

        // check for moving
        if (has_flag(tmp->win_flags, WindowFlags::Movable))
        {
            check_button(id, tmp->x, tmp->y, tmp->w, 20);

            if (context.active == id)
            {
                tmp->x += pos.first - context.mouse_x;
                tmp->y += pos.second - context.mouse_y;
            }
        }

        if (!tmp->collapsed)
            if (has_flag(tmp->win_flags, WindowFlags::Resizable))
            {
                const std::size_t &minW{name.size() * 7 + 32};

                // TODO: make sure these *_id don't match the id of any other widget.
                const std::size_t &resize_left_id{1 + id};
                const std::size_t &resize_right_id{2 + id};
                const std::size_t &resize_down_id{3 + id};

                const std::size_t &resize_right_down_id{4 + id};
                const std::size_t &resize_left_down_id{5 + id};

                // check for resizing
                check_button(resize_right_id, tmp->x + tmp->w - 10,
                             tmp->y + 20, 10, tmp->h - 10);
                check_button(resize_down_id, tmp->x + 10,
                             tmp->y + tmp->h - 10, tmp->w - 20, 10);
                check_button(resize_right_down_id, tmp->x + tmp->w - 10,
                             tmp->y + tmp->h - 10, 10, 10);

                if (context.active == resize_right_down_id)
                {
                    if (int const &dx{pos.first - context.mouse_x}; tmp->w >= minW || dx >= 0)
                        tmp->w = tmp->w + dx;
                    if (int const &dy{pos.second - context.mouse_y}; tmp->h >= 40 || dy >= 0)
                        tmp->h = tmp->h + dy;
                }

                if (context.active == resize_right_id)
                    if (int const &dx{pos.first - context.mouse_x}; tmp->w >= minW || dx >= 0) // bigger than double of the height of the title bar
                        tmp->w = tmp->w + dx;

                if (context.active == resize_down_id)
                    if (int const &dy{pos.second - context.mouse_y}; tmp->h >= 40 || dy >= 0)
                        tmp->h = tmp->h + dy;
            }

        context.mouse_x = pos.first;
        context.mouse_y = pos.second;

        tmp->next_y = tmp->y + 20;

        // drawing
        if (has_flag(tmp->win_flags, WindowFlags::OpaqueBackground))
        {
            if (!tmp->collapsed)
                draw_rect(win, {tmp->x, tmp->y, tmp->w, tmp->h}, {50, 50, 50, 255}); // body
            if (has_flag(tmp->win_flags, WindowFlags::WithTitleBar))
                draw_rect(win, {tmp->x, tmp->y, tmp->w, 20}, theme.idle_color); // title bar
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
        if (!context.on_window)
        {
            tnt::logger::debug("Calling tnt::ImGui::End() without "
                               "calling tnt::ImGui::Begin() before!!");
            return;
        }

        context.on_window = false;

        {
            window_data *last{get_last_win()};
            last->next_y = 0;
            last->menu_called = false;
        }

        if (!context.mouse_down)
            context.active = 0;
        else
        {
            if (context.active == 0)
                context.active = -1;
        }

        SDL_PumpEvents();
        update_context();
    }

    bool BeginSection(Window const &win, std::string_view text) noexcept
    {
        window_data *tmp{get_last_win()};
        const int w{tmp->w - 10}; // 10 padding to left, 10 to right

        // 40 = 5 + 5 left/right padding for +/-, 10 right padding for text, 10 +/- size.
        if (tmp->collapsed || tmp->next_y + theme.section_h > tmp->y + tmp->h ||
            tmp->w < static_cast<int>(text.size() * 7 + 40))
            return false;

        const std::size_t id{im_hash(tmp->title + text.data())};
        context.last_section = id;

        check_button(id, tmp->next_x + 5, tmp->next_y + 5, 10, 10);

        if (context.active == id)
            context.sections[id] = !context.sections[id];

        draw_rect(win, {tmp->next_x - 5, tmp->next_y, w, theme.section_h}, theme.idle_color);
        draw_rect(win, {tmp->next_x + 5, tmp->next_y + 8, 10, 4}, {255, 255, 255});

        if (!context.sections[id])
            draw_rect(win, {tmp->next_x + 8, tmp->next_y + 5, 4, 10}, {255, 255, 255});
        else
            tmp->next_x = tmp->next_x + 10;

        draw_text(win, text.data(), tmp->next_x + 20, tmp->next_y + 1);
        tmp->next_y = tmp->next_y + 30;

        return context.sections[id];
    }

    void EndSection() noexcept
    {
        window_data *tmp{get_last_win()};

        if (context.sections[context.last_section] && !tmp->collapsed)
            tmp->next_x = tmp->next_x - 10;
    }

    void BeginList(bool indexed) noexcept
    {
        ++get_last_win()->list_indent_level;
    }

    void EndList() noexcept
    {
        if (window_data * tmp{get_last_win()};
            tmp->list_indent_level <= 0)
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
            tmp->next_y = tmp->next_y + theme.font_size + 10;
        }
    }

    bool button(Window const &win, std::string_view text) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->next_y + theme.button_h > tmp->y + tmp->h ||
            tmp->w < theme.button_w + 10)
            return false;

        const std::size_t id{im_hash(tmp->title + text.data())};

        theme.button_w = static_cast<int>(text.size()) * 7 + 10;
        theme.button_h = 10 + theme.font_size;

        check_button(id, tmp->next_x, tmp->next_y, theme.button_w, theme.button_h);

        {
            const SDL_Rect dst{tmp->next_x, tmp->next_y, theme.button_w, theme.button_h};
            SDL_Color widgetColor;

            if (context.hot == id && context.active == id)
                widgetColor = theme.active_color;
            else
                widgetColor = theme.idle_color;

            draw_rect(win, dst, widgetColor);
        }
        draw_text(win, text.data(), tmp->next_x + 5, tmp->next_y + 5, theme.text_color, theme.font_size);

        tmp->next_y = tmp->next_y + 30;

        if (context.hot == id && context.active == id)
            return true;
        return false;
    }

    bool slider_int(Window const &win, int min_, int max_,
                    int *value) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->next_y + theme.slider_h > tmp->y + tmp->h ||
            tmp->w < theme.slider_w)
            return false;

        char temp[8];
        auto const &[ptr, ec]{std::to_chars(temp, temp + std::size(temp), *value)};
        const std::size_t &id{im_hash(tmp->title + temp)};

        const int &ypos{((theme.slider_h - theme.slider_thumb_size) * (*value - min_)) / (max_ - min_)};
        const int &offset{theme.slider_w - theme.slider_thumb_size};

        const SDL_Rect &thumb{tmp->next_x + (offset / 2), tmp->next_y + (offset / 2) + ypos, theme.slider_thumb_size,
                              theme.slider_thumb_size};

        check_button(id, tmp->next_x, tmp->next_y, theme.slider_w, theme.slider_h);

        draw_rect(win, {tmp->next_x, tmp->next_y, theme.slider_w, theme.slider_h + offset}, theme.static_color);
        draw_rect(win, thumb, theme.idle_color);

        bool ret{false};

        if (context.active == id)
        {
            int mousePos{context.mouse_y - (tmp->next_y + (offset / 2))};
            if (mousePos < 0)
                mousePos = 0;
            if (mousePos > (theme.slider_h - 1))
                mousePos = (theme.slider_h - 1);
            if (int const &v{min_ + (mousePos * (max_ - min_)) / (theme.slider_h - 1)}; v != *value)
            {
                *value = v;
                ret = true;
            }
        }

        tmp->next_y = tmp->next_y + theme.slider_h + 5;

        return ret;
    }

    bool slider_float(Window const &win, float min_, float max_,
                      float *value) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->next_y + theme.slider_h > tmp->y + tmp->h ||
            tmp->w < theme.slider_w)
            return false;

        char temp[8];
        auto const &[ptr, ec]{std::to_chars(temp, temp + std::size(temp), *value)};
        const std::size_t &id{im_hash(tmp->title + temp)};

        const int &ypos{static_cast<int>(((theme.slider_h - theme.slider_thumb_size) * (*value - min_)) /
                                         (max_ - min_))};
        const int &offset{theme.slider_w - theme.slider_thumb_size};

        check_button(id, tmp->next_x, tmp->next_y, theme.slider_w, theme.slider_h);

        draw_rect(win, {tmp->next_x, tmp->next_y, theme.slider_w, theme.slider_h + offset}, theme.static_color);

        const SDL_Rect &thumb{tmp->next_x + (offset / 2), tmp->next_y + (offset / 2) + ypos, theme.slider_thumb_size,
                              theme.slider_thumb_size};

        draw_rect(win, thumb, theme.idle_color);

        bool ret{false};

        if (context.active == id)
        {
            int mousePos{context.mouse_y - (tmp->next_y + (offset / 2))};
            if (mousePos < 0)
                mousePos = 0;
            if (mousePos > (theme.slider_h - 1))
                mousePos = (theme.slider_h - 1);
            if (float const &v{static_cast<float>(min_ + (mousePos * (max_ - min_)) / (theme.slider_h - 1))}; v != *value)
            {
                *value = v;
                ret = true;
            }
        }

        tmp->next_y = tmp->next_y + theme.slider_h + 5;

        return ret;
    }

    bool hslider_int(Window const &win, std::string_view text, int min_, int max_,
                     int *value) noexcept
    {
        window_data *tmp{get_last_win()};

        // 40 = 10 for left padding, 20 for distance between text and slider, 10 for right padding.
        const int &w{tmp->w - static_cast<int>(7 * text.size() + 40)};

        // 50 + 10 + 20, 50 -> slider minimal width, 10 -> padding from the left of the window, 20 -> distance between the slider and it's text
        if (tmp->collapsed || tmp->next_y + theme.hslider_h > tmp->y + tmp->h ||
            tmp->w < static_cast<int>(80 + 7 * text.size()))
            return false;

        const std::size_t &id{im_hash(tmp->title + text.data())};

        const bool &ret{hslider_basei(win, tmp, id, tmp->next_x, w, min_, max_, value)};

        draw_text(win, text.data(), tmp->next_x + w + 20, tmp->next_y);

        tmp->next_y = tmp->next_y + theme.hslider_h + 5;

        return ret;
    } // namespace tnt::ImGui

    bool hslider_float(Window const &win, std::string_view text, float min_, float max_,
                       float *value) noexcept
    {
        window_data *tmp{get_last_win()};

        // 40 = 10 for left padding, 20 for distance between text and slider, 10 for right padding.
        const int &w{tmp->w - static_cast<int>(7 * text.size() + 40)};

        // 50 + 10 + 20, 50 -> slider minimal width, 10 -> padding from the left of the window, 20 -> distance between the slider and it's text
        if (tmp->collapsed || tmp->next_y + theme.hslider_h > tmp->y + tmp->h ||
            tmp->w < static_cast<int>(80 + 7 * text.size()))
            return false;

        const std::size_t &id{im_hash(tmp->title + text.data())};
        const bool &ret{hslider_basef(win, tmp, id, tmp->next_x, w, min_, max_, value)};

        draw_text(win, text.data(), tmp->next_x + w + 20, tmp->next_y);

        tmp->next_y = tmp->next_y + theme.hslider_h + 5;

        return ret;
    }

    bool hslider_int2(Window const &win, std::string_view text,
                      int min_, int max_, int *value1, int *value2) noexcept
    {
        window_data *tmp{get_last_win()};

        // 50 = 10 for left padding, 20 for distance between text and slider,
        // 10 for right padding, 10 for distance between sliders.
        const int &w{(tmp->w - static_cast<int>(7 * text.size())) / 2 - 25};

        // 50 + 10 + 20, 50 -> slider minimal width, 10 -> padding from the left of the window, 20 -> distance between the slider and it's text
        if (tmp->collapsed || tmp->next_y + theme.hslider_h > tmp->y + tmp->h ||
            tmp->w < static_cast<int>(80 + 7 * text.size()))
            return false;

        const std::size_t &id1{im_hash(tmp->title + text.data() + "x")};
        const std::size_t &id2{im_hash(tmp->title + text.data() + "y")};

        const bool &ret1{hslider_basei(win, tmp, id1, tmp->next_x, w, min_, max_, value1)};
        const bool &ret2{hslider_basei(win, tmp, id2, tmp->next_x + w + 10, w, min_, max_, value2)};

        draw_text(win, text.data(), tmp->next_x + 2 * w + 0, tmp->next_y);

        tmp->next_y = tmp->next_y + theme.hslider_h + 5;

        return (ret1 || ret2);
    }

    bool hslider_float2(Window const &win, std::string_view text,
                        float min_, float max_, float *value1, float *value2) noexcept
    {
        window_data *tmp{get_last_win()};

        // 50 = 10 for left padding, 20 for distance between text and slider,
        // 10 for right padding, 10 for distance between sliders.
        const int &w{(tmp->w - static_cast<int>(7 * text.size())) / 2 - 25};

        // 50 + 10 + 20, 50 -> slider minimal width, 10 -> padding from the left of the window, 20 -> distance between the slider and it's text
        if (tmp->collapsed || tmp->next_y + theme.hslider_h > tmp->y + tmp->h ||
            tmp->w < static_cast<int>(80 + 7 * text.size()))
            return false;

        const std::size_t &id1{im_hash(tmp->title + text.data() + "x")};
        const std::size_t &id2{im_hash(tmp->title + text.data() + "y")};

        const bool ret1{hslider_basef(win, tmp, id1, tmp->next_x, w, min_, max_, value1)};
        const bool ret2{hslider_basef(win, tmp, id2, tmp->next_x + w + 10, w, min_, max_, value2)};

        draw_text(win, text.data(), tmp->next_x + 2 * w + 20, tmp->next_y);

        tmp->next_y = tmp->next_y + theme.hslider_h + 5;

        return (ret1 || ret2);
    }

    bool hslider_vec(Window const &win, std::string_view text,
                     float min1, float max1, float min2, float max2,
                     tnt::Vector *value) noexcept
    {
        window_data *tmp{get_last_win()};

        // 50 = 10 for left padding, 20 for distance between text and slider,
        // 10 for right padding, 10 for distance between sliders.
        const int &w{(tmp->w - static_cast<int>(7 * text.size())) / 2 - 25};

        // 50 + 10 + 20, 50 -> slider minimal width, 10 -> padding from the left of the window, 20 -> distance between the slider and it's text
        if (tmp->collapsed || tmp->next_y + theme.hslider_h > tmp->y + tmp->h ||
            tmp->w < static_cast<int>(80 + 7 * text.size()))
            return false;

        auto &[x_, y_]{*value};

        const std::size_t &id1{im_hash(tmp->title + text.data() + "x")};
        const std::size_t &id2{im_hash(tmp->title + text.data() + "y")};

        const bool ret1{hslider_basef(win, tmp, id1, tmp->next_x, w, min1, max1, &x_)};
        const bool ret2{hslider_basef(win, tmp, id2, tmp->next_x + w + 10, w, min2, max2, &y_)};

        if (x_ != value->x)
            value->x = x_;
        if (y_ != value->y)
            value->y = y_;

        draw_text(win, text.data(), tmp->next_x + 2 * w + 20, tmp->next_y);

        tmp->next_y = tmp->next_y + theme.hslider_h + 5;

        return (ret1 || ret2);
    }

    bool menu_button(Window const &win, std::string_view text) noexcept
    {
        window_data *tmp{get_last_win()};

        const int &x{tmp->x + tmp->menu_index * theme.menu_spacing + 7 * tmp->menu_txt_size};

        if (tmp->collapsed || x + static_cast<int>(text.size() + 1) * 7 > tmp->x + tmp->w)
            return false;

        const std::size_t &id{im_hash(tmp->title + text.data())};

        check_button( // 24 = 20 + 4 -> window title + menu_item padding
            id, x, tmp->next_y + 20,
            static_cast<int>(text.size()) * 7, theme.font_size + 6);
        draw_text(win, text.data(), x + 10, tmp->next_y + 20);
        logger::info("x: {}", x + 10);

        ++tmp->menu_index;
        tmp->last_menu_txt_size = tmp->menu_txt_size;
        tmp->menu_txt_size += static_cast<int>(text.size());

        return (context.hot == id && context.active == id);
    }

    bool menu_item(Window const &win, std::string_view text) noexcept
    {
        window_data *tmp{get_last_win()};

        const int &x{tmp->x + tmp->menu_index * theme.menu_spacing + 7 * tmp->last_menu_txt_size};

        // 100 = 30 -> padding from left and right + 70 -> size of 10 letters (with the current font)
        if (tmp->collapsed || x + 100 > tmp->x + tmp->w)
            return false;

        const std::size_t &id{im_hash(tmp->title + text.data())};

        check_button(id, x, tmp->next_y, 100, theme.font_size + 10);

        if (context.hot == id && context.active == id)
            draw_rect(win, {x, tmp->next_y, 100, theme.font_size + 10}, {0, 0, 0, 200});

        draw_text(win, text.data(), x + 15, tmp->next_y + 4);

        ++tmp->context_menu_index;
        tmp->next_y += theme.font_size + 24;

        return (context.hot == id);
    }

    bool checkbox(Window const &win, std::string_view text, bool *value) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->next_y + theme.checkbox_size + 10 > tmp->y + tmp->h)
            return false;

        const std::size_t &id{im_hash(tmp->title + text.data())};

        check_button(id, tmp->next_x, tmp->next_y, theme.checkbox_size, theme.checkbox_size);

        draw_rect(win, {tmp->next_x, tmp->next_y, theme.checkbox_size, theme.checkbox_size},
                  {130, 130, 130, 255});
        draw_text(win, text.data(), tmp->next_x + theme.checkbox_size + 10, tmp->next_y);
        tmp->next_y = tmp->next_y + theme.checkbox_size + 10;

        if (context.hot == id && context.active == id)
            *value = !(*value);

        if (*value)
        {
            const SDL_Rect &box{tmp->next_x, tmp->next_y, theme.checkbox_size, theme.checkbox_size};
            SDL_RenderCopy(win.getRenderer(), theme.checkbox_tick, nullptr, &box);
            return true;
        }

        return false;
    }

    void progress_bar(Window const &win, std::string_view text, int min_, int max_,
                      int *value) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->w < theme.pbar_w ||
            tmp->next_y + theme.pbar_h > tmp->y + tmp->h)
            return;
        const int &xpos{theme.pbar_w * (*value - min_) / (max_ - min_)};

        draw_rect(win, {tmp->x + 10, tmp->next_y, theme.pbar_w, theme.pbar_h},
                  theme.idle_color);
        draw_text(win, text.data(), tmp->x + theme.pbar_w + 20, tmp->next_y);
        draw_rect(win, {tmp->x + 12, tmp->next_y + 2, xpos, theme.pbar_h - 4}, theme.active_color);

        tmp->next_y = tmp->next_y + theme.pbar_h + 10;
    }

    void newline() noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->next_y + theme.font_size > tmp->y + tmp->h)
            return;
        tmp->next_y = tmp->next_y + theme.font_size;
    }

    void text(Window const &win, std::string_view text) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->w < 10 + 7 * text.size() ||
            tmp->next_y + theme.font_size + 5 > tmp->y + tmp->h)
            return;
        draw_text(win, text.data(), tmp->next_x, tmp->next_y);
        tmp->next_y = tmp->next_y + theme.font_size + 5;
    }

    void colored_text(Window const &win, std::string_view text,
                      unsigned char r, unsigned char g,
                      unsigned char b, unsigned char a) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed || tmp->w < 10 + 7 * text.size() ||
            tmp->next_y + theme.font_size + 5 > tmp->y + tmp->h)
            return;
        draw_text(win, text.data(), tmp->next_x, tmp->next_y, {r, g, b, a});
        tmp->next_y = tmp->next_y + theme.font_size + 5;
    }

    void list_item(Window const &win, std::string_view text) noexcept
    {
        window_data *tmp{get_last_win()};
        if (tmp->collapsed ||
            tmp->list_indent_level * 10 + text.size() * theme.font_size < tmp->w ||
            tmp->next_y + theme.font_size > tmp->y + tmp->h)
            return;
        const int &xpos{tmp->x + tmp->list_indent_level * 10};
        if (context.lists_text.find(text.data()) != context.lists_text.cend())
            context.lists_text[text.data()] = load_text(win, text.data());
        draw_text(win, text.data(), xpos, tmp->next_y);
        tmp->next_y = tmp->next_y + theme.font_size;
    }
} // namespace tnt::ImGui

/////////////////
// tnt_imgui_* //
/////////////////

void tnt_imgui_init(tnt::Window const &win) noexcept
{
    // TODO: move w and h to context
    tnt::ImGui::theme.w = win.getWidth();
    tnt::ImGui::theme.h = win.getHeight();

    tnt::ImGui::theme.font_data = TTF_OpenFont(tnt::ImGui::theme.default_font.data(),
                                               tnt::ImGui::theme.font_size);
    tnt::ImGui::theme.bg = win.getClearColor();

    tnt::ImGui::theme.checkbox_tick = tnt::ImGui::load_image(win, "assets/tick.png");

    tnt::ImGui::make_context();
    tnt::ImGui::theme.init = true;
}