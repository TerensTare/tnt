#ifndef TNT_VECTOR_HPP
#define TNT_VECTOR_HPP

//#if __cplusplus > 201703L
//#include <compare>
//#endif

#include <ostream>
#include "math/Easings.hpp"

// TODO: add noexcept at more places, if possible.
// TODO: add a ScalableVector struct, which has x, y and z coordinates, and uses z as a scale of itself.

// TODO(maybe):
// add constexpr on all functions ??

namespace tnt
{
struct Vector
{
    explicit constexpr Vector(float _x = 0.f, float _y = 0.f) noexcept : x{_x}, y{_y} {}

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
    float Magnitude() const noexcept { return sqrt(x * x + y * y); }

    void Normalize() noexcept
    {
        float mag{Magnitude()};
        x /= mag;
        y /= mag;
    }

    Vector Normalized() const
    {
        float mag{Magnitude()};
        return Vector{x / mag, y / mag};
    }

    // operators
    Vector operator-() const
    {
        return Vector{-x, -y};
    }

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

    bool operator<(const Vector &rhs) const noexcept { return std::tie(x, y) < std::tie(rhs.x, rhs.y); }
    bool operator>(const Vector &rhs) const noexcept { return ((x > rhs.x) && (y > rhs.y)); }

    //#if __cplusplus > 201703L // operator<=> isn't available before C++20
    //		auto operator<=>(Vector const&) const = default;
    //#endif

    friend std::ostream &operator<<(std::ostream &os, Vector const &rhs);

    float x;
    float y;
};

inline Vector operator+(const Vector &lhs, const Vector &rhs) { return Vector{lhs.x + rhs.x, lhs.y + rhs.y}; }
inline Vector operator-(const Vector &lhs, const Vector &rhs) { return Vector{lhs.x - rhs.x, lhs.y - rhs.y}; }
inline Vector operator*(const Vector &vec, const float &num) { return Vector{vec.x * num, vec.y * num}; }
inline Vector operator/(const Vector &vec, const float &num) { return Vector{vec.x / num, vec.y / num}; }

inline float Dot(const Vector &lhs, const Vector &rhs)
{
    float angleCos = static_cast<float>((lhs.x * rhs.x + lhs.y * rhs.y) / (lhs.Magnitude() * rhs.Magnitude()));
    float angle = RadianToDegree(acosf(angleCos));
    return angle;
}

inline float Cross(Vector const &lhs, Vector const &rhs)
{
    return ((lhs.x * rhs.y) - (lhs.y * rhs.x));
}

// TODO: make this available inside the struct.
inline Vector RotateVector(Vector &vec, float angle)
{
    float radAngle = static_cast<float>(DegreeToRadian(angle));
    return Vector{
        static_cast<float>(vec.x * cosf(radAngle) - vec.y * sinf(radAngle)),
        static_cast<float>(vec.x * sinf(radAngle) + vec.y * cosf(radAngle))};
}

inline Vector nlerp(Vector left, Vector right, float pct)
{
    return lerp<Vector>(left, right, pct).Normalized();
}

inline bool operator<=(const Vector &lhs, const Vector &rhs) noexcept { return ((lhs.x <= rhs.x) && (lhs.y <= rhs.y)); }
inline bool operator>=(const Vector &lhs, const Vector &rhs) noexcept { return ((lhs.x >= rhs.x) && (lhs.y >= rhs.y)); }
inline bool operator==(const Vector &lhs, const Vector &rhs) noexcept {return ((fabs(lhs.x - rhs.x) < 1e-11) && (fabs(lhs.y - rhs.y) < 1e-11)); }

#if __cplusplus <= 201703L || defined(TNT_ALLOW_DEPRECATIONS) // operator!= is deprecated in C++20
inline bool operator!=(const Vector &lhs, const Vector &rhs) noexcept
{
    return ((lhs.x != rhs.x) || (lhs.y != rhs.y));
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

#endif //!TNT_VECTOR_HPP