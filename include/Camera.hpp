#ifndef TNT_CAMERA_HPP
#define TNT_CAMERA_HPP

#include <SDL2/SDL_rect.h>
#include "math/Vector.hpp"

// Thx Penaz91
// https://github.com/Penaz91/2DGD_F0TH

// TODO: screenshake
// TODO: different Camera types.
// TODO: Lua support.

// TODO(maybe):
// make Camera's be classes rather than struct-s.
// remove the base Camera class.
// derive from StaticCamera.

namespace tnt
{
// Camera uses global coordinates.
// The Camera class is deprecated in favour to the new StaticCamera, HorizontalCamera, FullTrackingCamera class-es, and will be removed as soon as the new Camera-s will be written and fully tested.
class [[deprecated("The Camera class is deprecated in favour to the new StaticCamera, HorizontalCamera, FullTrackingCamera class-es, and will be removed as soon as the new Camera-s will be written and fully tested.")]] Camera
{
public:
    Camera(float x, float y, float w, float h);

    // momentally
    Camera(Camera const &) = delete;
    Camera(Camera &&) = delete;
    Camera &operator=(Camera const &) = delete;
    Camera &operator=(Camera &&) = delete;

    void Move(float x, float y);
    void Move(Vector const &v);

    void MoveX(float x);
    void MoveY(float y);

    void MoveTo(float x, float y);
    void MoveTo(Vector const &v);

    void CenterTo(float x, float y);
    void CenterTo(Vector const &v);

    SDL_FRect FBounds() const noexcept;
    SDL_Rect Bounds() const noexcept;

private:
    SDL_FRect bounds;
};

class StaticCamera
{
public:
    StaticCamera(float x, float y, float w, float h);

    // momentally
    StaticCamera(StaticCamera const &) = delete;
    StaticCamera(StaticCamera &&) = delete;
    StaticCamera &operator=(StaticCamera const &) = delete;
    StaticCamera &operator=(StaticCamera &&) = delete;

    SDL_FRect FBounds() const noexcept;
    SDL_Rect Bounds() const noexcept;

private:
    SDL_FRect bounds;
};

class HorizontalCamera
{
public:
    HorizontalCamera(float x, float y, float w, float h);

    // momentally
    HorizontalCamera(HorizontalCamera const &) = delete;
    HorizontalCamera(HorizontalCamera &&) = delete;
    HorizontalCamera &operator=(HorizontalCamera const &) = delete;
    HorizontalCamera &operator=(HorizontalCamera &&) = delete;

    void Move(float x, [[maybe_unused]] float y);
    void Move(Vector const &v);

    void MoveTo(float x, [[maybe_unused]] float y);
    void MoveTo(Vector const &v);

    void CenterTo(float x, [[maybe_unused]] float y);
    void CenterTo(Vector const &v);

    SDL_FRect FBounds() const noexcept;
    SDL_Rect Bounds() const noexcept;

private:
    SDL_FRect bounds;
};

struct FullTrackingCamera
{
    FullTrackingCamera(float x, float y, float w, float h);

    // momentally
    FullTrackingCamera(FullTrackingCamera const &) = delete;
    FullTrackingCamera(FullTrackingCamera &&) = delete;
    FullTrackingCamera &operator=(FullTrackingCamera const &) = delete;
    FullTrackingCamera &operator=(FullTrackingCamera &&) = delete;

    void Move(float x, float y);
    void Move(Vector const &v);

    void MoveX(float x);
    void MoveY(float y);

    void MoveTo(float x, float y);
    void MoveTo(Vector const &v);

    void CenterTo(float x, float y);
    void CenterTo(Vector const &v);

private:
    SDL_FRect bounds;
};
} // namespace tnt

#endif //!TNT_CAMERA_HPP