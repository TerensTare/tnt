#ifndef TNT_GRAPHICS_HPP
#define TNT_GRAPHICS_HPP

namespace tnt::detail::gfx
{
/// @brief Initalizes graphics and other SDL2-related stuff.
/// The function returns true if everything is initialized successfully, false otherwise.
/// @return bool
/// @note This function should only used by tnt::Window and should NOT be called by the user.
bool Init() noexcept;

/// @brief Closes all SDL2-related stuff correctly.
/// @return bool
/// @note This function should only used by tnt::Window and should NOT be called by the user.
void Quit() noexcept;
} // namespace tnt::detail::gfx

#endif //! TNT_GRAPHICS_HPP