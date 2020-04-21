#include "core/InputManager.hpp"

namespace tnt::input
{
int keyLength;
int mX, mY;
bool inited{false};

Uint32 currentMouse;
Uint32 prevMouse;
Uint32 lastMouse;
const Uint8 *currentkb;
std::vector<Uint8> prevkb;

void init()
{
    currentkb = SDL_GetKeyboardState(&keyLength);
    prevkb.assign(currentkb, currentkb + keyLength);
    if (!inited)
        inited = true;
}
} // namespace tnt::input

void tnt::input::close() noexcept
{
    for (auto it{prevkb.begin()}; it != prevkb.end(); ++it)
        it = prevkb.erase(it);
    prevkb.clear();
    inited = false;
}

bool tnt::input::keyDown(SDL_Scancode key) noexcept
{
    return (currentkb[key] != 0);
}

bool tnt::input::keyPressed(SDL_Scancode key) noexcept
{
    return ((prevkb[key] == 0) && (currentkb[key] != 0));
}

bool tnt::input::keyReleased(SDL_Scancode key) noexcept
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

bool tnt::input::mouseButtonDown(Uint32 button) noexcept
{
    Uint32 mask{0};
    GET_MOUSE_MASK(button, mask);
    return ((currentMouse & mask) != 0);
}

bool tnt::input::mouseButtonPressed(Uint32 button) noexcept
{
    Uint32 mask{0};
    GET_MOUSE_MASK(button, mask);
    return (((prevMouse & mask) == 0) && ((currentMouse & mask) != 0));
}

bool tnt::input::mouseButtonReleased(Uint32 button) noexcept
{
    Uint32 mask{0};
    GET_MOUSE_MASK(button, mask);
    return (((prevMouse & mask) != 0) && ((currentMouse & mask) == 0));
}

#undef GET_MOUSE_MASK

unsigned tnt::input::lastMouseButton() noexcept
{
    for (int i{0}; i < 5; ++i)
        if (currentMouse & (1 << i))
            lastMouse = i;
    for (int i{0}; i < 5; ++i)
        if (prevMouse & (1 << i))
            lastMouse = i;

    return lastMouse;
}

void tnt::input::updateCurrent()
{
    if (!inited)
        tnt::input::init();
    currentMouse = SDL_GetMouseState(&mX, &mY);
}

void tnt::input::updatePrevious()
{
    if (!inited)
        tnt::input::init();
    prevkb.assign(currentkb, currentkb + keyLength);
    prevMouse = currentMouse;
}

std::pair<int, int> tnt::input::mousePosition() noexcept
{
    return std::make_pair(mX, mY);
}