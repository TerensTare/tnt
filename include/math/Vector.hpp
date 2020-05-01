#ifndef TNT_VECTOR_HPP
#define TNT_VECTOR_HPP

//#if __cplusplus > 201703L
//#include <compare>
//#endif

#include <ostream>

#include "math/Easings.hpp"

// TODO: add noexcept at more places, if possible.
// TODO: add a ScalableVector struct, which has x, y and z coordinates, and uses
// z as a scale of itself.

// TODO(maybe):
// add constexpr on all functions ??

namespace tnt
{
struct Vector
{
    explicit constexpr Vector(float _x = 0.f, float _y = 0.f) noexcept
        : x{_x}, y{_y} {}

    // template <typename T1, typename T2 = T1,
    // 		  typename = std::enable_if_t<
    // 			  std::is_convertible_v<T1, float> &&
    // 			  std::is_convertible_v<T2, float> &&
    // 			  !std::is_pointer_v<T1> &&
    // 			  !std::is_pointer_v<T2>>>
    // Vector(T1 _x = 0, T2 _y = 0)
    // 	: x{static_cast<float>(_x)},
    // 	  y{static_cast<float>(_y)} {} // TODO(experimental)

    Vector(const Vector &) = default;
    Vector(Vector &&) = default;

    float MagnitudeSqr() const noexcept { return x * x + y * y; }
    float Magnitude() const noexcept { return sqrtf(x * x + y * y); }

    void Normalize() noexcept(noexcept(Magnitude() > FLT_EPSILON))
    {
        float mag{Magnitude()};
        x /= mag;
        y /= mag;
    }

    Vector Normalized() const noexcept(noexcept(Magnitude() > FLT_EPSILON))
    {
        float mag{Magnitude()};
        return Vector{x / mag, y / mag};
    }

    // operators
    Vector operator-() const noexcept { return Vector{-x, -y}; }

    Vector &operator+=(const Vector &rhs) noexcept
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector &operator-=(const Vector &rhs) noexcept
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vector &operator*=(const Vector &rhs) noexcept
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    Vector &operator/=(const Vector &rhs) noexcept
    {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    Vector &operator=(const Vector &rhs) noexcept
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    Vector &operator=(Vector &&rhs) noexcept = default;

    bool operator<(const Vector &rhs) const noexcept
    {
        return std::tie(x, y) < std::tie(rhs.x, rhs.y);
    }

    bool operator>(const Vector &rhs) const noexcept
    {
        return ((x > rhs.x) && (y > rhs.y));
    }

    // #if __cplusplus > 201703L // operator<=> isn't available before C++20
    //     auto operator<=>(Vector const &) const = default;
    // #endif

    friend std::ostream &operator<<(std::ostream &os, Vector const &rhs);

    float x;
    float y;
};

inline Vector operator+(const Vector &lhs, const Vector &rhs) noexcept
{
    return Vector{lhs.x + rhs.x, lhs.y + rhs.y};
}
inline Vector operator-(const Vector &lhs, const Vector &rhs) noexcept
{
    return Vector{lhs.x - rhs.x, lhs.y - rhs.y};
}
inline Vector operator*(const Vector &vec, const float &num) noexcept
{
    return Vector{vec.x * num, vec.y * num};
}
inline Vector operator/(const Vector &vec, const float &num) noexcept
{
    return Vector{vec.x / num, vec.y / num};
}

inline float Magnitude(float x, float y) noexcept { return sqrtf(x * x + y * y); }

inline float Dot(const Vector &lhs, const Vector &rhs) noexcept(noexcept(lhs.Magnitude() * rhs.Magnitude() > FLT_EPSILON))
{
    float angleCos =
        static_cast<float>((lhs.x * rhs.x + lhs.y * rhs.y) /
                           (lhs.Magnitude() * rhs.Magnitude()));
    float angle = RadianToDegree(acosf(angleCos));
    return angle;
}

inline float Cross(Vector const &lhs, Vector const &rhs) noexcept
{
    return ((lhs.x * rhs.y) - (lhs.y * rhs.x));
}

// TODO: make this available inside the struct.
inline Vector RotateVector(Vector const &vec, float angle) noexcept
{
    float radAngle = static_cast<float>(DegreeToRadian(angle));
    return Vector{
        static_cast<float>(vec.x * cosf(radAngle) - vec.y * sinf(radAngle)),
        static_cast<float>(vec.x * sinf(radAngle) +
                           vec.y * cosf(radAngle))};
}

inline Vector nlerp(Vector const &left, Vector const &right, float pct) noexcept
{
    return lerp<Vector>(left, right, pct).Normalized();
}

inline bool operator<=(const Vector &lhs, const Vector &rhs) noexcept
{
    return (((lhs.x - rhs.x) <= FLT_EPSILON) && ((lhs.y - rhs.y) <= FLT_EPSILON));
}

inline bool operator>=(const Vector &lhs, const Vector &rhs) noexcept
{
    return (((lhs.x - rhs.x) >= FLT_EPSILON) && ((lhs.y - rhs.y) >= FLT_EPSILON));
}

inline bool operator==(const Vector &lhs, const Vector &rhs) noexcept
{
    return ((fabsf(lhs.x - rhs.x) < FLT_EPSILON) && (fabsf(lhs.y - rhs.y) < FLT_EPSILON));
}

#if __cplusplus <= 201703L || defined(TNT_ALLOW_DEPRECATIONS) // operator!= is deprecated in C++20
inline bool operator!=(const Vector &lhs, const Vector &rhs) noexcept
{
    return (fabsf((lhs.x - rhs.x) > FLT_EPSILON) || (fabsf(lhs.y - rhs.y) > FLT_EPSILON));
}
#endif

inline static constexpr Vector VECTOR_ZERO{0.f, 0.f};
inline static constexpr Vector VECTOR_ONE{1.f, 1.f};
inline static constexpr Vector VECTOR_UP{0.f, 1.f};
inline static constexpr Vector VECTOR_RIGHT{1.f, 0.f};
inline static constexpr Vector VECTOR_DOWN{0.f, -1.f};
inline static constexpr Vector VECTOR_LEFT{-1.f, 0.f};
} // namespace tnt

inline std::ostream &tnt::operator<<(std::ostream &os, Vector const &rhs)
{
    os << "{" << rhs.x << ", " << rhs.y << "}";
    return os;
}

#endif //! TNT_VECTOR_HPP