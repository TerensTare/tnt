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
        /// @brief Construct a new static (non-movable, only shake-able)
        /// camera from the arguments given.
        /// @param x_ The x position.
        /// @param y_ The y position.
        /// @param w_ The width of the camera.
        /// @param h_ The height of the camera.
        Camera(float x_, float y_, float w_, float h_) noexcept;

        /// @overload This constructor has the same exact behaviour as the other
        /// constructor, except that it uses a @c tnt::Rectangle.
        explicit Camera(tnt::Rectangle const &rect) noexcept;

        Camera(Camera const &) = delete;
        Camera(Camera &&) = delete;
        Camera &operator=(Camera const &) = delete;
        Camera &operator=(Camera &&) = delete;

        /// @brief Apply a shaking effect on the camera.
        /// @param time The time in ms for the effect to continue.
        /// @param intensity The intensity of the shaking effect.
        /// @note intensity should be between 0.0f and 1.0f.
        void Shake(float time, float intensity) noexcept;

        /// @return tnt::Rectangle
        Rectangle Bounds() const noexcept;

        float x, y, w, h;

    protected:
        bool shaking{false};
        float tmpX{0.f}, tmpY{0.f}; // used during shaking
    };

    /// @brief A camera that can only move horizontally.
    /// @note This struct uses global coordinates.
    struct HorizontalCamera : Camera
    {
        /// @brief Constructs a new horizontal camera from the arguments given.
        /// @param x_ The x position.
        /// @param y_ The y position.
        /// @param w_ The width of the camera.
        /// @param h_ The height of the camera.
        HorizontalCamera(float x_, float y_, float w_, float h_) noexcept;

        /// @overload This constructor has the same exact behaviour as the other
        /// constructor, except that it uses a @c tnt::Rectangle.
        explicit HorizontalCamera(tnt::Rectangle const &rect) noexcept;

        /// @brief Moves the camera horizontally by @c x units.
        /// @param x_ The number of units the camera should move horizontally.
        void Move(float x_, float) noexcept;

        /// @brief Moves the camera horizontally by using a @c tnt::Vector.
        /// @param v The Vector used to move the camera.
        void Move(Vector const &v) noexcept;

        /// @brief Moves the camera's @c x to @c x.
        /// @param x_ The destination position.
        void MoveTo(float x_, float) noexcept;

        /// @brief Moves the camera's @c x to @c v.x.
        /// @param v The Vector containing the destination position.
        void MoveTo(Vector const &v) noexcept;

        /// @brief Centers the camera to @c (x,defaultY).
        /// @param x_ The desired center of the camera.
        void CenterTo(float x_, float) noexcept;

        /// @brief Centers the camera to @c (v.x,defaultY).
        /// @param v The Vector containing the desired center of the camera.
        void CenterTo(Vector const &v) noexcept;
    };

    /// @brief A camera that can move horizontally and vertically.
    /// @note This struct uses global coordinates.
    struct FullTrackingCamera : Camera
    {
        /// @brief Constructs a new full tracking camera from the arguments given.
        /// @param x_ The x position.
        /// @param y_ The y position.
        /// @param w_ The width of the camera.
        /// @param h_ The height of the camera.
        FullTrackingCamera(float x_, float y_, float w_, float h_) noexcept;

        /// @overload This constructor has the same exact behaviour as the other
        /// constructor, except that it uses a @c tnt::Rectangle.
        explicit FullTrackingCamera(tnt::Rectangle const &rect) noexcept;

        /// @brief Moves the camera by @c (x,y) units.
        /// @param x_ The number of units the camera should move horizontally.
        /// @param y_ The number of units the camera should move vertically.
        void Move(float x_, float y_) noexcept;

        /// @brief Moves the camera by using a @c tnt::Vector.
        /// @param v The Vector used to move the camera.
        void Move(Vector const &v) noexcept;

        /// @brief Move only the @c x position of the camera.
        /// @param x_ The number of units to move the camera.
        void MoveX(float x_) noexcept;

        /// @brief Move only the @c y position of the camera.
        /// @param y_ The number of units to move the camera.
        void MoveY(float y_) noexcept;

        /// @brief Moves the camera to @c (x,y).
        /// @param x_ The horizontal destination.
        /// @param y_ The vertical destination.
        void MoveTo(float x_, float y_) noexcept;

        /// @brief Moves the camera to @c v.
        /// @param v The destination point.
        void MoveTo(Vector const &v) noexcept;

        /// @brief Centers the camera to @c (x,y).
        /// @param x_ The horizontal center.
        /// @param y_ The vertical center.
        void CenterTo(float x_, float y_) noexcept;

        /// @brief Centers the camera to @c v.
        /// @param v The desired center of the camera.
        void CenterTo(Vector const &v) noexcept;
    };

    /// @brief A concept representing basic Camera type requirements.
    template <typename T>
    concept camera = std::is_base_of_v<tnt::Camera, T> ||
                     std::is_same_v<tnt::Camera, T>;
} // namespace tnt

#endif //! TNT_CAMERA_HPP