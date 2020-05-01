// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "core/Camera.hpp"
#include "pcg/Random.hpp"

////////////
// Camera //
////////////

tnt::Camera::Camera(float x, float y, float w, float h)
    : bounds{x, y, w, h}, shaking{false}, tmpX{0.f}, tmpY{0.f} {}

tnt::Rectangle tnt::Camera::Bounds() const noexcept { return bounds; }

void tnt::Camera::Shake(long long time, float intensity) noexcept
{
    if (intensity > .1f)
    {
        if (!shaking)
        {
            tmpX = bounds.x;
            tmpY = bounds.y;
        }
        else
            shaking = true;
        intensity = intensity * 0.9f;
        float angle{randomFloat(0.f, 2 * PI)}; // random angle betwen [0, 360] degree
        float xOffset{bounds.h * std::cosf(angle)};
        float yOffset{bounds.w * std::sinf(angle)};
        bounds.x = bounds.x + xOffset;
        bounds.y = bounds.y + yOffset;
        return;
    }
    shaking = false;
    bounds.x = tmpX;
    bounds.y = tmpY;
}

///////////////////////
// Horizontal Camera //
///////////////////////

tnt::HorizontalCamera::HorizontalCamera(float x, float y, float w, float h)
    : Camera{x, y, w, h} {}

void tnt::HorizontalCamera::Move(float x, float) noexcept { bounds.x = bounds.x + x; }

void tnt::HorizontalCamera::Move(tnt::Vector const &v) noexcept
{
    bounds.x = bounds.x + v.x;
}

void tnt::HorizontalCamera::MoveTo(float x, float) noexcept { bounds.x = x; }

void tnt::HorizontalCamera::MoveTo(tnt::Vector const &v) noexcept { bounds.x = v.x; }

void tnt::HorizontalCamera::CenterTo(float x, float) noexcept
{
    bounds.x = (x - (bounds.w / 2));
}

void tnt::HorizontalCamera::CenterTo(tnt::Vector const &v) noexcept
{
    bounds.x = (v.x - (bounds.w / 2));
}

///////////////////////////
// Full Tracking Camera ///
///////////////////////////

tnt::FullTrackingCamera::FullTrackingCamera(float x, float y, float w, float h)
    : Camera{x, y, w, h} {}

void tnt::FullTrackingCamera::Move(float x, float y) noexcept
{
    bounds.x = bounds.x + x;
    bounds.y = bounds.y + y;
}

void tnt::FullTrackingCamera::Move(Vector const &v) noexcept
{
    bounds.x = bounds.x + v.x;
    bounds.y = bounds.y + v.y;
}

void tnt::FullTrackingCamera::MoveX(float x) noexcept { bounds.x = bounds.x + x; }

void tnt::FullTrackingCamera::MoveY(float y) noexcept { bounds.y = bounds.y + y; }

void tnt::FullTrackingCamera::MoveTo(float x, float y) noexcept
{
    bounds.x = x;
    bounds.y = y;
}

void tnt::FullTrackingCamera::MoveTo(Vector const &v) noexcept
{
    bounds.x = v.x;
    bounds.y = v.y;
}

void tnt::FullTrackingCamera::CenterTo(float x, float y) noexcept
{
    bounds.x = x - (bounds.w / 2);
    bounds.y = y - (bounds.h / 2);
}

void tnt::FullTrackingCamera::CenterTo(Vector const &v) noexcept
{
    bounds.x = v.x - (bounds.w / 2);
    bounds.y = v.y - (bounds.h / 2);
}