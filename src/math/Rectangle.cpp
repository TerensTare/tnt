// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <SDL2/SDL_rect.h>
#include "math/Rectangle.hpp"

tnt::Rectangle::operator SDL_Rect() noexcept
{
    return SDL_Rect{
        static_cast<int>(x), static_cast<int>(y),
        static_cast<int>(w), static_cast<int>(h)};
}

tnt::Rectangle::operator SDL_FRect() noexcept
{
    return SDL_FRect{x, y, w, h};
}

tnt::Rectangle::operator const SDL_Rect() const noexcept
{
    return SDL_Rect{
        static_cast<int>(x), static_cast<int>(y),
        static_cast<int>(w), static_cast<int>(h)};
}

tnt::Rectangle::operator const SDL_FRect() const noexcept
{
    return SDL_FRect{x, y, w, h};
}