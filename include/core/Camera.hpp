#ifndef TNT_CAMERA_HPP
#define TNT_CAMERA_HPP

#include "math/Rectangle.hpp"

// Thx Penaz91
// https://github.com/Penaz91/2DGD_F0TH

// TODO: screenshake
// TODO: MoveTo and CenterTo with lerp so that it looks nice.
// TODO(partial): different Camera types.
// TODO: Lua support.
// TODO: clamp Camera coordinates inside Scene's coordinates.

// TODO(maybe):
// make Camera's be classes rather than struct-s ??
// remove the ctor from all types derived from Camera ??
// create a NonStaticCamera class that stores a Vector offset,
// related to the Scene's origin
// and other Camera-s should derive from that ??
namespace tnt
{
// NOTE: Camera uses global coordinates.
struct Camera
{
    Camera(float x, float y, float w, float h);

    Camera(Camera const &) = delete;
    Camera(Camera &&) = delete;
    Camera &operator=(Camera const &) = delete;
    Camera &operator=(Camera &&) = delete;

    void Shake(long long time, float intensity) noexcept; // TODO: unimplemented

    Rectangle Bounds() const noexcept;

protected:
    bool shaking;
    float tmpX, tmpY; // used during shaking
    Rectangle bounds;
};

struct HorizontalCamera : Camera
{
    HorizontalCamera(float x, float y, float w, float h);

    void Move(float x, float) noexcept;
    void Move(Vector const &v) noexcept;

    void MoveTo(float x, float) noexcept;
    void MoveTo(Vector const &v) noexcept;

    void CenterTo(float x, float) noexcept;
    void CenterTo(Vector const &v) noexcept;
};

struct FullTrackingCamera : Camera
{
    FullTrackingCamera(float x, float y, float w, float h);

    void Move(float x, float y) noexcept;
    void Move(Vector const &v) noexcept;

    void MoveX(float x) noexcept;
    void MoveY(float y) noexcept;

    void MoveTo(float x, float y) noexcept;
    void MoveTo(Vector const &v) noexcept;

    void CenterTo(float x, float y) noexcept;
    void CenterTo(Vector const &v) noexcept;
};
} // namespace tnt

#endif //! TNT_CAMERA_HPP