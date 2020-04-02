#ifndef TNT_IMGUI_CONFIG_HPP
#define TNT_IMGUI_CONFIG_HPP

namespace tnt::ImGui
{

////////////
// global //
////////////

int get_window_w() noexcept;
int get_window_h() noexcept;

void set_bg_color(unsigned char r, unsigned char g,
                  unsigned char b, unsigned char a) noexcept;
unsigned char *get_bg_color() noexcept;

////////////
// button //
////////////

void set_button_w(int w) noexcept;
int get_button_w() noexcept;

void set_button_h(int h) noexcept;
int get_button_h() noexcept;

void set_button_idle_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) noexcept;
unsigned char *get_button_idle_color() noexcept;

void set_button_active_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) noexcept;
unsigned char *get_button_active_color() noexcept;
} // namespace tnt::ImGui

#endif //!TNT_IMGUI_CONFIG_HPP