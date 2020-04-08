#ifndef TNT_INPUT_MANAGER_HPP
#define TNT_INPUT_MANAGER_HPP

#include <vector>
#include <SDL2/SDL_events.h>

// TODO: lastKeyPressed.
// TODO: Joystick and GameController support.

// TODO(maybe):
// rename this header ??
// SDL_Scancode const& ??

namespace tnt::input
{

void close() noexcept;

bool keyDown(SDL_Scancode key) noexcept;
bool keyPressed(SDL_Scancode key) noexcept;
bool keyReleased(SDL_Scancode key) noexcept;

bool mouseButtonDown(Uint32 button) noexcept;
bool mouseButtonPressed(Uint32 button) noexcept;
bool mouseButtonReleased(Uint32 button) noexcept;

void updateCurrent();
void updatePrevious();

unsigned lastMouseButton() noexcept;

std::pair<int, int> mousePosition() noexcept;

} // namespace tnt::input

#endif //!TNT_INPUT_MANAGER_HPP