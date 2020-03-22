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
    ~InputManager() noexcept;

    bool KeyDown(SDL_Scancode key) const noexcept;
    bool KeyPressed(SDL_Scancode key) const noexcept;
    bool KeyReleased(SDL_Scancode key) const noexcept;

    bool MouseButtonDown(Uint32 button) const noexcept;
    bool MouseButtonPressed(Uint32 button) const noexcept;
    bool MouseButtonReleased(Uint32 button) const noexcept;

    void UpdateCurrentInput();
    void UpdatePreviousInput();

    unsigned LastMouseButton() noexcept;

    std::pair<int, int> MousePosition() const noexcept;

private:
    InputManager();

    int keyLength;
    int mX, mY;

    Uint32 currentMouse;
    Uint32 prevMouse;
    Uint32 lastMouse;
    const Uint8 *currentkb;
    std::vector<Uint8> prevkb;
};
} // namespace tnt

#endif //!INPUT_MANAGER_HPP