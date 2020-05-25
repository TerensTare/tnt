#ifndef TNT_IMGUI_CONFIG_HPP
#define TNT_IMGUI_CONFIG_HPP

#ifdef TNT_IMGUI_RUNTIME_CONFIG

typedef struct _TTF_Font TTF_Font;

namespace tnt::ImGui
{
    ////////////
    // global //
    ////////////

    int get_window_w() noexcept;
    int get_window_h() noexcept;

    unsigned char *get_bg_color() noexcept;
    unsigned char *get_font_color() noexcept;

    void set_font(char const *name) noexcept;
    TTF_Font *get_font() noexcept;

    ////////////
    // button //
    ////////////

    void set_button_idle_color(unsigned char r, unsigned char g,
                               unsigned char b, unsigned char a) noexcept;
    unsigned char *get_button_idle_color() noexcept;

    void set_button_active_color(unsigned char r, unsigned char g,
                                 unsigned char b, unsigned char a) noexcept;
    unsigned char *get_button_active_color() noexcept;

    ////////////
    // slider //
    ////////////

    void set_slider_color(unsigned char r, unsigned char g, unsigned char b,
                          unsigned char a) noexcept;
    unsigned char *get_slider_color() noexcept;

    void set_slider_thumb_color(unsigned char r, unsigned char g,
                                unsigned char b, unsigned char a) noexcept;
    unsigned char *get_slider_thumb_color() noexcept;

    ///////////////////////
    // horizontal slider //
    ///////////////////////

    void set_hslider_color(unsigned char r, unsigned char g, unsigned char b,
                           unsigned char a) noexcept;
    unsigned char *get_hslider_color() noexcept;

    void set_hslider_thumb_color(unsigned char r, unsigned char g,
                                 unsigned char b, unsigned char a) noexcept;
    unsigned char *get_hslider_thumb_color() noexcept;

    //////////////////
    // progress bar //
    //////////////////

    unsigned char *get_progressbar_idle_color() noexcept;
    void set_progressbar_idle_color(unsigned char r, unsigned char g,
                                    unsigned char b, unsigned char a) noexcept;

    unsigned char *get_progressbar_filled_color() noexcept;
    void set_progressbar_filled_color(unsigned char r, unsigned char g,
                                      unsigned char b, unsigned char a) noexcept;
} // namespace tnt::ImGui

#endif //! TNT_IMGUI_RUNTIME_CONFIG

#endif //! TNT_IMGUI_CONFIG_HPP