#ifndef TNT_IMGUI_CONFIG_HPP
#define TNT_IMGUI_CONFIG_HPP

namespace tnt::ImGui
{
    ////////////
    // global //
    ////////////

    int get_window_w() noexcept;
    int get_window_h() noexcept;

    // void set_bg_color(unsigned char r, unsigned char g,
    //                   unsigned char b, unsigned char a) noexcept;
    unsigned char *get_bg_color() noexcept;

    void set_font(char const *name) noexcept;
    char const *get_font() noexcept;

    void set_font_size(int size) noexcept;
    int get_font_size() noexcept;

    ////////////
    // button //
    ////////////

    void set_button_w(int w) noexcept;
    int get_button_w() noexcept;

    void set_button_h(int h) noexcept;
    int get_button_h() noexcept;

    void set_button_idle_color(unsigned char r, unsigned char g,
                               unsigned char b, unsigned char a) noexcept;
    unsigned char *get_button_idle_color() noexcept;

    void set_button_active_color(unsigned char r, unsigned char g,
                                 unsigned char b, unsigned char a) noexcept;
    unsigned char *get_button_active_color() noexcept;

    ////////////
    // slider //
    ////////////

    void set_slider_w(int w) noexcept;
    int get_slider_w() noexcept;

    void set_slider_h(int h) noexcept;
    int get_slider_h() noexcept;

    void set_slider_thumb_w(int w) noexcept;
    int get_slider_thumb_w() noexcept;

    void set_slider_thumb_h(int h) noexcept;
    int get_slider_thumb_h() noexcept;

    void set_slider_color(unsigned char r, unsigned char g, unsigned char b,
                          unsigned char a) noexcept;
    unsigned char *get_slider_color() noexcept;

    void set_slider_thumb_idle_color(unsigned char r, unsigned char g,
                                     unsigned char b, unsigned char a) noexcept;
    unsigned char *get_slider_thumb_idle_color() noexcept;

    void set_slider_thumb_active_color(unsigned char r, unsigned char g,
                                       unsigned char b,
                                       unsigned char a) noexcept;
    unsigned char *get_slider_thumb_active_color() noexcept;

    ///////////////////////
    // horizontal slider //
    ///////////////////////

    void set_hslider_w(int w) noexcept;
    int get_hslider_w() noexcept;

    void set_hslider_h(int h) noexcept;
    int get_hslider_h() noexcept;

    void set_hslider_thumb_w(int w) noexcept;
    int get_hslider_thumb_w() noexcept;

    void set_hslider_thumb_h(int h) noexcept;
    int get_hslider_thumb_h() noexcept;

    void set_hslider_color(unsigned char r, unsigned char g, unsigned char b,
                           unsigned char a) noexcept;
    unsigned char *get_hslider_color() noexcept;

    void set_hslider_thumb_idle_color(unsigned char r, unsigned char g,
                                      unsigned char b,
                                      unsigned char a) noexcept;
    unsigned char *get_hslider_thumb_idle_color() noexcept;

    void set_hslider_thumb_active_color(unsigned char r, unsigned char g,
                                        unsigned char b,
                                        unsigned char a) noexcept;
    unsigned char *get_hslider_thumb_active_color() noexcept;

} // namespace tnt::ImGui

#endif //! TNT_IMGUI_CONFIG_HPP