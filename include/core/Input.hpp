#ifndef TNT_INPUT_HPP
#define TNT_INPUT_HPP

#include <SDL2/SDL_events.h>

#include <vector>

// TODO: lastKeyPressed.
// TODO: Joystick and GameController support.

// TODO(maybe):
// SDL_Scancode const& ??

namespace tnt::input
{
/// @brief Makes input handling unavailable when called.
void close() noexcept;

/// @brief Checks if a keyboard key is being held down.
/// @param key The key to check.
/// @return true if @param key is held down.
bool keyDown(SDL_Scancode key) noexcept;

/// @brief Checks if a keyboard key is pressed.
/// @param key The key to check.
/// @return true if @param key is pressed.
bool keyPressed(SDL_Scancode key) noexcept;

/// @brief Checks if a keyboard key is released.
/// @param key The key to check.
/// @return true if @param key is released.
bool keyReleased(SDL_Scancode key) noexcept;

/// @brief Checks if a mouse button is being held down.
/// @param button The button to check.
/// @return true if @param button is being held down.
bool mouseButtonDown(Uint32 button) noexcept;

/// @brief Checks if a mouse button is pressed.
/// @param button The button to check.
/// @return true if @param button is pressed.
bool mouseButtonPressed(Uint32 button) noexcept;

/// @brief Checks if a mouse button is released.
/// @param button The button to check.
/// @return true if @param button is released.
bool mouseButtonReleased(Uint32 button) noexcept;

/// Updates the current input handles.
void updateCurrent();

/// @brief Updates the last input handles.
void updatePrevious();

/// @return The last mouse button pressed.
unsigned lastMouseButton() noexcept;

/// @return The position of the mouse as a std::pair<int, int>.
std::pair<int, int> mousePosition() noexcept;

} // namespace tnt::input

#endif //! TNT_INPUT_HPP