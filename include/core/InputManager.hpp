#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <vector>
#include <SDL2/SDL_events.h>

namespace tnt
{

class InputManager
{
public:
    static InputManager &This();

    bool KeyDown(SDL_Scancode key) const noexcept;
    bool KeyPressed(SDL_Scancode key) const noexcept;
    bool KeyReleased(SDL_Scancode key) const noexcept;

    bool MouseButtonDown(Uint32 button) const noexcept;
    bool MouseButtonPressed(Uint32 button) const noexcept;
    bool MouseButtonReleased(Uint32 button) const noexcept;

    void UpdateCurrentInput();
    void UpdatePreviousInput();

    std::pair<int, int> MousePosition() const noexcept;

private:
    InputManager();
    ~InputManager() noexcept;

    const Uint8 *currentkb;
    std::vector<Uint8> prevkb;
    int keyLength;

    Uint32 currentMouse;
    Uint32 prevMouse;
    int mX, mY;
};
} // namespace tnt

#endif //!INPUT_MANAGER_HPP