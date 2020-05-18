// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "core/Input.hpp"

namespace tnt::input
{
    int keyLength{0};
    int mX{0}, mY{0};
    bool inited{false};

    Uint32 currentMouse{0};
    Uint32 prevMouse{0};
    Uint32 lastMouse{0};
    const Uint8 *currentkb{nullptr};
    std::vector<Uint8> prevkb;

    void init()
    {
        currentkb = SDL_GetKeyboardState(&keyLength);
        prevkb.assign(currentkb, currentkb + keyLength);
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

SDL_Scancode tnt::input::lastKeyPressed(const SDL_Event &e) noexcept
{
    return e.key.keysym.scancode;
}

bool tnt::input::mouseButtonDown(Uint32 button) noexcept
{
    Uint32 mask{1u << button};
    return ((currentMouse & mask) != 0);
}

bool tnt::input::mouseButtonPressed(Uint32 button) noexcept
{
    Uint32 mask{1u << button};
    return (((prevMouse & mask) == 0) && ((currentMouse & mask) != 0));
}

bool tnt::input::mouseButtonReleased(Uint32 button) noexcept
{
    Uint32 mask{1u << button};
    return (((prevMouse & mask) != 0) && ((currentMouse & mask) == 0));
}

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