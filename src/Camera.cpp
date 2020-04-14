// This is an independent project of an individual developer. Dear PVS-Studio,
// please check it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com

#include "Camera.hpp"

////////////
// Camera //
////////////

tnt::Camera::Camera(float x, float y, float w, float h)
    : bounds{SDL_FRect{x, y, w, h}}
{}

SDL_FRect tnt::Camera::FBounds() const noexcept { return SDL_FRect{bounds}; }

SDL_Rect tnt::Camera::Bounds() const noexcept
{
    return SDL_Rect{(int)bounds.x, (int)bounds.y, (int)bounds.w, (int)bounds.h};
}

///////////////////////
// Horizontal Camera //
///////////////////////

tnt::HorizontalCamera::HorizontalCamera(float x, float y, float w, float h)
    : Camera{x, y, w, h}
{}

void tnt::HorizontalCamera::Move(float x, float) { bounds.x = bounds.x + x; }

void tnt::HorizontalCamera::Move(tnt::Vector const& v)
{
    bounds.x = bounds.x + v.x;
}

void tnt::HorizontalCamera::MoveTo(float x, float) { bounds.x = x; }

void tnt::HorizontalCamera::MoveTo(tnt::Vector const& v) { bounds.x = v.x; }

void tnt::HorizontalCamera::CenterTo(float x, float)
{
    bounds.x = (x - (bounds.w / 2));
}

void tnt::HorizontalCamera::CenterTo(tnt::Vector const& v)
{
    bounds.x = (v.x - (bounds.w / 2));
}

///////////////////////////
// Full Tracking Camera ///
///////////////////////////

tnt::FullTrackingCamera::FullTrackingCamera(float x, float y, float w, float h)
    : Camera{x, y, w, h}
{}

void tnt::FullTrackingCamera::Move(float x, float y)
{
    bounds.x = bounds.x + x;
    bounds.y = bounds.y + y;
}

void tnt::FullTrackingCamera::Move(Vector const& v)
{
    bounds.x = bounds.x + v.x;
    bounds.y = bounds.y + v.y;
}

void tnt::FullTrackingCamera::MoveX(float x) { bounds.x = bounds.x + x; }

void tnt::FullTrackingCamera::MoveY(float y) { bounds.y = bounds.y + y; }

void tnt::FullTrackingCamera::MoveTo(float x, float y)
{
    bounds.x = x;
    bounds.y = y;
}

void tnt::FullTrackingCamera::MoveTo(Vector const& v)
{
    bounds.x = v.x;
    bounds.y = v.y;
}

void tnt::FullTrackingCamera::CenterTo(float x, float y)
{
    bounds.x = x - (bounds.w / 2);
    bounds.y = y - (bounds.h / 2);
}

void tnt::FullTrackingCamera::CenterTo(Vector const& v)
{
    bounds.x = v.x - (bounds.w / 2);
    bounds.y = v.y - (bounds.h / 2);
}
// void CenterTo(Vector const &v);