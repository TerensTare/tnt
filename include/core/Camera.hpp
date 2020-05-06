#ifndef TNT_CAMERA_HPP
#define TNT_CAMERA_HPP

#include "math/Rectangle.hpp"

// Thx Penaz91
// https://github.com/Penaz91/2DGD_F0TH

// TODO: screenshake
// TODO: Zooming
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

/// @brief A static camera which also serves as a base class for other camera types.
/// @note This struct uses global coordinates.
struct Camera
{
    /// @brief Constructs a new static (non-movable, only shake-able)
    /// camera from the arguments given.
    /// @param x The x position.
    /// @param y The y position.
    /// @param w The width of the camera.
    /// @param h The height of the camera.
    Camera(float x, float y, float w, float h);

    Camera(Camera const &) = delete;
    Camera(Camera &&) = delete;
    Camera &operator=(Camera const &) = delete;
    Camera &operator=(Camera &&) = delete;

    /// @brief Applies a shaking effect on the camera.
    /// @param time The time in ms for the effect to continue.
    /// @param intensity The intensity of the shaking effect.
    /// @note intensity should be between 0.0f and 1.0f.
    void Shake(long long time, float intensity) noexcept;

    /// @return tnt::Rectangle
    Rectangle Bounds() const noexcept;

protected:
    bool shaking;
    float tmpX, tmpY; // used during shaking
    Rectangle bounds;
};

/// @brief A camera that can only move horizontally.
/// @note This struct uses global coordinates.
struct HorizontalCamera : Camera
{
    /// @brief Constructs a new horizontal camera from the arguments given.
    /// @param x The x position.
    /// @param y The y position.
    /// @param w The width of the camera.
    /// @param h The height of the camera.
    HorizontalCamera(float x, float y, float w, float h);

    /// @brief Moves the camera horizontally by @em x units.
    /// @param x The number of units the camera should move horizontally.
    void Move(float x, float) noexcept;

    /// @brief Moves the camera horizontally by using a @em tnt::Vector.
    /// @param v The Vector used to move the camera.
    void Move(Vector const &v) noexcept;

    /// @brief Moves the camera's @em x to @em x.
    /// @param x The destination position.
    void MoveTo(float x, float) noexcept;

    /// @brief Moves the camera's @em x to @em v.x.
    /// @param v The Vector containing the destination position.
    void MoveTo(Vector const &v) noexcept;

    /// @brief Centers the camera to @em (x,defaultY).
    /// @param x The desired center of the camera.
    void CenterTo(float x, float) noexcept;

    /// @brief Centers the camera to @em (v.x,defaultY).
    /// @param v The Vector containing the desired center of the camera.
    void CenterTo(Vector const &v) noexcept;
};

/// @brief A camera that can move horizontally and vertically.
/// @note This struct uses global coordinates.
struct FullTrackingCamera : Camera
{
    /// @brief Constructs a new full tracking camera from the arguments given.
    /// @param x The x position.
    /// @param y The y position.
    /// @param w The width of the camera.
    /// @param h The height of the camera.
    FullTrackingCamera(float x, float y, float w, float h);

    /// @brief Moves the camera by @em (x,y) units.
    /// @param x The number of units the camera should move horizontally.
    /// @param y The number of units the camera should move vertically.
    void Move(float x, float y) noexcept;

    /// @brief Moves the camera by using a @em tnt::Vector.
    /// @param v The Vector used to move the camera.
    void Move(Vector const &v) noexcept;

    /// @brief Move only the @em x position of the camera.
    /// @param x The number of units to move the camera.
    void MoveX(float x) noexcept;

    /// @brief Move only the @em y position of the camera.
    /// @param y The number of units to move the camera.
    void MoveY(float y) noexcept;

    /// @brief Moves the camera to @em (x,y).
    /// @param x The horizontal destination.
    /// @param y The vertical destination.
    void MoveTo(float x, float y) noexcept;

    /// @brief Moves the camera to @em v.
    /// @param v The destination point.
    void MoveTo(Vector const &v) noexcept;

    /// @brief Centers the camera to @em (x,y).
    /// @param x The horizontal center.
    /// @param y The vertical center.
    void CenterTo(float x, float y) noexcept;

    /// @brief Centers the camera to @em v.
    /// @param v The desired center of the camera.
    void CenterTo(Vector const &v) noexcept;
};
} // namespace tnt

#endif //! TNT_CAMERA_HPP