#include "InputManager.hpp"

tnt::InputManager &tnt::InputManager::This()
{
    static InputManager inst;
    return inst;
}

tnt::InputManager::InputManager()
{
    currentkb = SDL_GetKeyboardState(&keyLength);
    prevkb.assign(currentkb, currentkb + keyLength);
}

tnt::InputManager::~InputManager() noexcept
{
    for (auto it{prevkb.begin()}; it != prevkb.end(); ++it)
        it = prevkb.erase(it);
    prevkb.clear();
}

bool tnt::InputManager::KeyDown(SDL_Scancode key) const noexcept
{
    return (currentkb[key] != 0);
}

bool tnt::InputManager::KeyPressed(SDL_Scancode key) const noexcept
{
    return ((prevkb[key] == 0) && (currentkb[key] != 0));
}

bool tnt::InputManager::KeyReleased(SDL_Scancode key) const noexcept
{
    return ((prevkb[key] != 0) && (currentkb[key] == 0));
}

#define GET_MOUSE_MASK(button, mask)    \
    if ((button) == 0) /*left*/         \
        (mask) = (SDL_BUTTON_LMASK);    \
    else if ((button) == 1) /*middle*/  \
        (mask) = (SDL_BUTTON_MMASK);    \
    else if ((button) == 2) /*right*/   \
        (mask) = (SDL_BUTTON_RMASK);    \
    else if ((button) == 3) /*back*/    \
        (mask) = (SDL_BUTTON_X1MASK);   \
    else if ((button) == 4) /*forward*/ \
        (mask) = (SDL_BUTTON_X2MASK);

bool tnt::InputManager::MouseButtonDown(Uint32 button) const noexcept
{
    Uint32 mask{0};
    GET_MOUSE_MASK(button, mask);

    return ((currentMouse & mask) != 0);
}

bool tnt::InputManager::MouseButtonPressed(Uint32 button) const noexcept
{
    Uint32 mask{0};
    GET_MOUSE_MASK(button, mask);
    return (((prevMouse & mask) == 0) && ((currentMouse & mask) != 0));
}

bool tnt::InputManager::MouseButtonReleased(Uint32 button) const noexcept
{
    Uint32 mask{0};
    GET_MOUSE_MASK(button, mask);
    return (((prevMouse & mask) != 0) && ((currentMouse & mask) == 0));
}

void tnt::InputManager::UpdateCurrentInput()
{
    currentMouse = SDL_GetMouseState(&mX, &mY);
}

void tnt::InputManager::UpdatePreviousInput()
{
    prevkb.assign(currentkb, currentkb + keyLength);
    prevMouse = currentMouse;
}

std::pair<int, int> tnt::InputManager::MousePosition() const noexcept
{
    return std::make_pair(mX, mY);
}