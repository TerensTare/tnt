#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Vector.hpp"

namespace tnt
{
// Camera uses global coordinates.
struct Camera
{
    Camera(float x, float y, float w, float h);

    // momentally
    Camera(Camera const &) = delete;
    Camera(Camera &&) = delete;
    Camera &operator=(Camera const &) = delete;

    void Move(float x, float y);
    void Move(Vector v);

    void MoveX(float x);
    void MoveY(float y);

    void MoveTo(float x, float y);
    void MoveTo(Vector v);

    void CenterTo(float x, float y);
    void CenterTo(Vector v);

    SDL_FRect &FBounds() const noexcept;
    SDL_Rect &Bounds() const noexcept;

private:
    SDL_FRect bounds;
};
} // namespace tnt

#endif //!CAMERA_HPP