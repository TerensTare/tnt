#ifndef TNT_INPUT_HPP
#define TNT_INPUT_HPP

#include <utility>

#include <SDL2/SDL_events.h>
#include "core/Config.hpp"

// TODO: Better Joystick and GameController support.

namespace tnt::input
{
    /// @brief Checks if a keyboard key is being held down.
    /// @param key The key to check.
    /// @return true if @c key is held down.
    TNT_API bool keyDown(SDL_Scancode key) noexcept;

    /// @brief Checks if a keyboard key is pressed.
    /// @param key The key to check.
    /// @return true if @c key is pressed.
    TNT_API bool keyPressed(SDL_Scancode key) noexcept;

    /// @brief Checks if a keyboard key is released.
    /// @param key The key to check.
    /// @return true if @c key is released.
    TNT_API bool keyReleased(SDL_Scancode key) noexcept;

    /// @brief Get the last key pressed by checking the event handler.
    /// @param e The event handler to check.
    /// @return SDL_Scancode
    TNT_API SDL_Scancode lastKeyPressed(SDL_Event const &e) noexcept;

    /// @brief Checks if a mouse button is being held down.
    /// @param button The button to check.
    /// @return true if @c button is being held down.
    TNT_API bool mouseButtonDown(Uint32 button) noexcept;

    /// @brief Checks if a mouse button is pressed.
    /// @param button The button to check.
    /// @return true if @c button is pressed.
    TNT_API bool mouseButtonPressed(Uint32 button) noexcept;

    /// @brief Checks if a mouse button is released.
    /// @param button The button to check.
    /// @return true if @c button is released.
    TNT_API bool mouseButtonReleased(Uint32 button) noexcept;

    /// @brief Updates the current input handles.
    TNT_API void updateCurrent();

    /// @brief Updates the last input handles.
    TNT_API void updatePrevious();

    /// @brief Updates joystick-related datas.
    TNT_API void updateJoystick();

    /// @brief Updates all the input handles.
    TNT_API void Update();

    /// @return The last mouse button pressed.
    TNT_API unsigned lastMouseButton() noexcept;

    /// @return The position of the mouse as a @c std::pair<int, int>.
    TNT_API std::pair<int, int> mousePosition() noexcept;

    /// @brief Set the default sensitivity to all the connected joysticks.
    /// @note The default sensitivity of the joystick when connected is 8000.
    /// @param sense The desired sensitivity.
    TNT_API void setDefaultDeadZone(Sint16 sense) noexcept;

    /// @brief Set the sensitivity of a joystick.
    /// @param id The id of the joystick.
    /// @param sense The sensitivity of the joystick (from 0 to 32767).
    TNT_API void setDeadZone(SDL_JoystickID id, Sint16 sense) noexcept;

    /// @brief Get the sensitivity of the joystick.
    /// @param id The id of the joystick.
    /// @return Sint16
    TNT_API Sint16 getDeadZone(SDL_JoystickID id) noexcept;
} // namespace tnt::input

#endif //! TNT_INPUT_HPP