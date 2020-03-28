#ifndef TNT_CAMERA_HPP
#define TNT_CAMERA_HPP

#include <SDL2/SDL_rect.h>
#include "math/Vector.hpp"

// Thx Penaz91
// https://github.com/Penaz91/2DGD_F0TH

// TODO: screenshake
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
// Camera uses global coordinates.
// The Camera class is deprecated in favour to the new StaticCamera, HorizontalCamera, FullTrackingCamera class-es, and will be removed as soon as the new Camera-s will be written and fully tested.
// In fact, it may serve as a base class for the other Camera class-es.
struct Camera
{
    Camera(float x, float y, float w, float h);

    Camera(Camera const &) = delete;
    Camera(Camera &&) = delete;
    Camera &operator=(Camera const &) = delete;
    Camera &operator=(Camera &&) = delete;

    void Screenshake() noexcept; // TODO: unimplemented

    SDL_FRect FBounds() const noexcept;
    SDL_Rect Bounds() const noexcept;

protected:
    SDL_FRect bounds;
};

struct HorizontalCamera : Camera
{
    HorizontalCamera(float x, float y, float w, float h);

    void Move(float x, float);
    void Move(Vector const &v);

    void MoveTo(float x, float);
    void MoveTo(Vector const &v);

    void CenterTo(float x, float);
    void CenterTo(Vector const &v);
};

struct FullTrackingCamera : Camera // (maybe) derive from HorizontalCamera
{
    FullTrackingCamera(float x, float y, float w, float h);

    void Move(float x, float y);
    void Move(Vector const &v);

    void MoveX(float x);
    void MoveY(float y);

    void MoveTo(float x, float y);
    void MoveTo(Vector const &v);

    void CenterTo(float x, float y);
    void CenterTo(Vector const &v);
};
} // namespace tnt

#endif //!TNT_CAMERA_HPP