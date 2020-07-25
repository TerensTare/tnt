// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <vector>
#include "core/Input.hpp"

namespace tnt::input
{
    inline struct input_handle_t
    {
        inline input_handle_t() noexcept(noexcept(
            prevkb.assign(currentkb, currentkb + keyLength)))
        {
            prevkb.assign(currentkb, currentkb + keyLength);
        }

        int keyLength{0};
        int mX{0}, mY{0};

        Uint32 currentMouse{0};
        Uint32 prevMouse{0};
        Uint32 lastMouse{0};
        const Uint8 *currentkb{SDL_GetKeyboardState(&keyLength)};
        std::vector<Uint8> prevkb;
    } input_handle;
} // namespace tnt::input

bool tnt::input::keyDown(SDL_Scancode key) noexcept
{
    return (input_handle.currentkb[key] != 0);
}

bool tnt::input::keyPressed(SDL_Scancode key) noexcept
{
    return ((input_handle.prevkb[key] == 0) && (input_handle.currentkb[key] != 0));
}

bool tnt::input::keyReleased(SDL_Scancode key) noexcept
{
    return ((input_handle.prevkb[key] != 0) && (input_handle.currentkb[key] == 0));
}

SDL_Scancode tnt::input::lastKeyPressed(const SDL_Event &e) noexcept
{
    return e.key.keysym.scancode;
}

bool tnt::input::mouseButtonDown(Uint32 button) noexcept
{
    return ((input_handle.currentMouse & (1u << button)) != 0);
}

bool tnt::input::mouseButtonPressed(Uint32 button) noexcept
{
    return (((input_handle.prevMouse & (1u << button)) == 0) &&
            ((input_handle.currentMouse & (1u << button)) != 0));
}

bool tnt::input::mouseButtonReleased(Uint32 button) noexcept
{
    return (((input_handle.prevMouse & (1u << button)) != 0) &&
            ((input_handle.currentMouse & (1u << button)) == 0));
}

unsigned tnt::input::lastMouseButton() noexcept
{
    for (Uint32 i{0}; i < 5; ++i)
        if (input_handle.currentMouse & (1 << i))
            input_handle.lastMouse = i;
    for (Uint32 i{0}; i < 5; ++i)
        if (input_handle.prevMouse & (1 << i))
            input_handle.lastMouse = i;

    return input_handle.lastMouse;
}

void tnt::input::updateCurrent()
{
    input_handle.currentMouse = SDL_GetMouseState(&input_handle.mX, &input_handle.mY);
}

void tnt::input::updatePrevious()
{
    input_handle.prevkb.assign(input_handle.currentkb, input_handle.currentkb + input_handle.keyLength);
    input_handle.prevMouse = input_handle.currentMouse;
}

std::pair<int, int> tnt::input::mousePosition() noexcept
{
    return std::make_pair(input_handle.mX, input_handle.mY);
}