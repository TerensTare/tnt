#ifndef TNT_IMGUI_CONFIG_HPP
#define TNT_IMGUI_CONFIG_HPP

#ifdef TNT_IMGUI_RUNTIME_CONFIG

namespace tnt::ImGui
{
    int get_window_w() noexcept;
    int get_window_h() noexcept;

    void set_font(char const *name) noexcept;
} // namespace tnt::ImGui

#endif //! TNT_IMGUI_RUNTIME_CONFIG
#endif //! TNT_IMGUI_CONFIG_HPP