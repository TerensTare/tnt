// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Camera.hpp"

tnt::Camera::Camera(float x, float y, float w, float h)
    : bounds{SDL_FRect{x, y, w, h}} {}

void tnt::Camera::Move(float x, float y)
{
    bounds.x = bounds.x + x;
    bounds.y = bounds.y + y;
}

void tnt::Camera::Move(tnt::Vector const &v)
{
    bounds.x = bounds.x + v.x;
    bounds.y = bounds.y + v.y;
}

void tnt::Camera::MoveX(float x)
{
    bounds.x = bounds.x + x;
}

void tnt::Camera::MoveY(float y)
{
    bounds.y = bounds.y + y;
}

void tnt::Camera::MoveTo(float x, float y)
{
    bounds.x = x;
    bounds.y = y;
}

void tnt::Camera::MoveTo(tnt::Vector const &v)
{
    bounds.x = v.x;
    bounds.y = v.y;
}

void tnt::Camera::CenterTo(float x, float y)
{
    bounds.x = (x - (bounds.w / 2));
    bounds.y = (y - (bounds.h / 2));
}

void tnt::Camera::CenterTo(tnt::Vector const &v)
{
    bounds.x = (v.x - (bounds.w / 2));
    bounds.y = (v.y - (bounds.h / 2));
}

SDL_FRect tnt::Camera::FBounds() const noexcept { return SDL_FRect{bounds}; }

SDL_Rect tnt::Camera::Bounds() const noexcept
{
    return SDL_Rect{(int)bounds.x, (int)bounds.y, (int)bounds.w, (int)bounds.h};
}