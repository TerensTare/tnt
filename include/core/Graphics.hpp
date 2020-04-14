#ifndef TNT_GRAPHICS_HPP
#define TNT_GRAPHICS_HPP

namespace tnt::detail::gfx
{
    bool Init() noexcept;
    void Quit() noexcept;
    inline static bool init;
} // namespace tnt::detail::gfx

#endif //! TNT_GRAPHICS_HPP