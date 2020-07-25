// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "core/Camera.hpp"
#include "pcg/Random.hpp"

////////////
// Camera //
////////////

tnt::Camera::Camera(float x_, float y_, float w_, float h_) noexcept
    : x{x_}, y{y_}, w{w_}, h{h_} {}

tnt::Camera::Camera(tnt::Rectangle const &rect) noexcept
    : x{rect.x}, y{rect.y}, w{rect.w}, h{rect.h} {}

tnt::Rectangle tnt::Camera::Bounds() const noexcept { return {x, y, w, h}; }

void tnt::Camera::Shake(float time, float intensity) noexcept
{
    if (intensity > .1f)
    {
        if (!shaking)
        {
            tmpX = x;
            tmpY = y;
        }
        else
            shaking = true;
        intensity = intensity * 0.9f;
        float angle{randomFloat(0.f, 2 * PI)}; // random angle betwen [0, 360] degree
        float xOffset{h * std::cosf(angle)};
        float yOffset{w * std::sinf(angle)};
        x = x + xOffset;
        y = y + yOffset;
        return;
    }
    shaking = false;
    x = tmpX;
    y = tmpY;
}

///////////////////////
// Horizontal Camera //
///////////////////////

tnt::HorizontalCamera::HorizontalCamera(float x_, float y_, float w_, float h_) noexcept
    : Camera{x_, y_, w_, h_} {}

tnt::HorizontalCamera::HorizontalCamera(tnt::Rectangle const &rect) noexcept
    : Camera{rect} {}

void tnt::HorizontalCamera::Move(float x_, float) noexcept { x = x + x_; }

void tnt::HorizontalCamera::Move(tnt::Vector const &v) noexcept
{
    x = x + v.x;
}

void tnt::HorizontalCamera::MoveTo(float x_, float) noexcept { x = x_; }

void tnt::HorizontalCamera::MoveTo(tnt::Vector const &v) noexcept { x = v.x; }

void tnt::HorizontalCamera::CenterTo(float x_, float) noexcept
{
    x = (x_ - (w / 2));
}

void tnt::HorizontalCamera::CenterTo(tnt::Vector const &v) noexcept
{
    x = (v.x - (w / 2));
}

///////////////////////////
// Full Tracking Camera ///
///////////////////////////

tnt::FullTrackingCamera::FullTrackingCamera(float x_, float y_, float w_, float h_) noexcept
    : Camera{x_, y_, w_, h_} {}

tnt::FullTrackingCamera::FullTrackingCamera(tnt::Rectangle const &rect) noexcept
    : Camera{rect} {}

void tnt::FullTrackingCamera::Move(float x_, float y_) noexcept
{
    x = x + x_;
    y = y + y_;
}

void tnt::FullTrackingCamera::Move(Vector const &v) noexcept
{
    x = x + v.x;
    y = y + v.y;
}

void tnt::FullTrackingCamera::MoveX(float x_) noexcept { x = x + x_; }

void tnt::FullTrackingCamera::MoveY(float y_) noexcept { y = y + y_; }

void tnt::FullTrackingCamera::MoveTo(float x_, float y_) noexcept
{
    x = x_;
    y = y_;
}

void tnt::FullTrackingCamera::MoveTo(Vector const &v) noexcept
{
    x = v.x;
    y = v.y;
}

void tnt::FullTrackingCamera::CenterTo(float x_, float y_) noexcept
{
    x = x_ - (w / 2);
    y = y_ - (h / 2);
}

void tnt::FullTrackingCamera::CenterTo(Vector const &v) noexcept
{
    x = v.x - (w / 2);
    y = v.y - (h / 2);
}