#ifndef TNT_VECTOR_HPP
#define TNT_VECTOR_HPP

#if defined(__cpp_impl_three_way_comparison) && __has_include(<compare>)
#include <compare>
#endif

#include <ostream>

#include "math/MathUtils.hpp"

// TODO:
// add a ScalableVector struct, which has x, y and z coordinates, and uses
// z as a scale of itself.
// add math operators (*, /, *=, /=) with this signature (float, Vector const&)

namespace tnt {
struct Vector {
  constexpr Vector(float _x = 0.f, float _y = 0.f) noexcept
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

  constexpr float MagnitudeSqr() const noexcept { return x * x + y * y; }
  constexpr float Magnitude() const noexcept {
    return Q_sqrt(x * x + y * y);
  }

  constexpr void Normalize() noexcept {
    float const &rsqrt{Q_rsqrt(x * x + y * y)};
    x *= rsqrt;
    y *= rsqrt;
  }

  constexpr Vector Normalized() const noexcept {
    return Vector{x * Q_rsqrt(x * x + y * y), y * Q_rsqrt(x * x + y * y)};
  }

  // operators
  constexpr Vector operator-() const noexcept { return Vector{-x, -y}; }

  constexpr Vector &operator+=(const Vector &rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  constexpr Vector &operator-=(const Vector &rhs) noexcept {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

  constexpr Vector &operator*=(const Vector &rhs) noexcept {
    x *= rhs.x;
    y *= rhs.y;
    return *this;
  }

  constexpr Vector &operator*=(float rhs) noexcept {
    x *= rhs;
    y *= rhs;
    return *this;
  }

  constexpr Vector &operator/=(const Vector &rhs) noexcept {
    x /= rhs.x;
    y /= rhs.y;
    return *this;
  }

  constexpr Vector &operator/=(float rhs) noexcept {
    x /= rhs;
    y /= rhs;
    return *this;
  }

  constexpr Vector &operator=(const Vector &rhs) noexcept {
    x = rhs.x;
    y = rhs.y;
    return *this;
  }

#if defined(__cpp_impl_three_way_comparison) && __has_include(<compare>)
  constexpr auto operator<=>(Vector const &rhs) const noexcept {
    if (auto const &cmp{x <=> rhs.x}; cmp != 0)
      return cmp;
    else
      return y <=> rhs.y;
  }
#else
  constexpr bool operator<(const Vector &rhs) const noexcept {
    return (x - rhs.x < FLT_EPSILON) ||
           ((x - rhs.x == FLT_EPSILON) && (y - rhs.y < FLT_EPSILON));
  }

  constexpr bool operator>(const Vector &rhs) const noexcept {
    return (x - rhs.x > FLT_EPSILON) ||
           ((x - rhs.x == FLT_EPSILON) && (y - rhs.y > FLT_EPSILON));
  }
#endif

  float x;
  float y;
};

constexpr Vector operator+(const Vector &lhs,
                                  const Vector &rhs) noexcept {
  return Vector{lhs.x + rhs.x, lhs.y + rhs.y};
}

constexpr Vector operator-(const Vector &lhs,
                                  const Vector &rhs) noexcept {
  return Vector{lhs.x - rhs.x, lhs.y - rhs.y};
}

constexpr Vector operator*(const Vector &vec,
                                  const float &num) noexcept {
  return Vector{vec.x * num, vec.y * num};
}

constexpr Vector operator/(const Vector &vec,
                                  const float &num) noexcept {
  return Vector{vec.x / num, vec.y / num};
}

constexpr float Magnitude(float x, float y) noexcept {
  return Q_sqrt(x * x + y * y);
}

inline float AngleOf(Vector const &lhs, Vector const &rhs) noexcept {
  float const &angleCos{static_cast<float>(
      (lhs.x * rhs.x + lhs.y * rhs.y) / (lhs.Magnitude() * rhs.Magnitude()))};
  return RadianToDegree(acosf(angleCos));
}

constexpr float Dot(Vector const &lhs, Vector const &rhs) noexcept {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}

constexpr float Cross(Vector const &lhs, Vector const &rhs) noexcept {
  return lhs.x * rhs.y - lhs.y * rhs.x;
}

// TODO: make this available inside the struct.
inline Vector RotateVector(Vector const &vec, float angle) noexcept {
  float const &radAngle{static_cast<float>(DegreeToRadian(angle))};
  return Vector{
      static_cast<float>(vec.x * cosf(radAngle) - vec.y * sinf(radAngle)),
      static_cast<float>(vec.x * sinf(radAngle) + vec.y * cosf(radAngle))};
}

inline Vector nlerp(Vector const &left, Vector const &right,
                    float pct) noexcept {
  return lerp<Vector>(left, right, pct).Normalized();
}

#if !defined(__cpp_impl_three_way_comparison) && !__has_include(<compare>)
constexpr bool operator<=(Vector const &lhs,
                                 Vector const &rhs) noexcept {
  return !(lhs > rhs);
}

constexpr bool operator>=(const Vector &lhs,
                                 const Vector &rhs) noexcept {
  return !(lhs < rhs);
}
#endif

constexpr bool operator==(const Vector &lhs,
                                 const Vector &rhs) noexcept {
  return (lhs <= rhs) && (lhs >= rhs);
}

#if !defined(__cpp_impl_three_way_comparison) && !__has_include(<compare>)
constexpr bool operator!=(const Vector &lhs,
                                 const Vector &rhs) noexcept {
  return !(lhs == rhs);
}
#endif

inline constexpr Vector VECTOR_ZERO{0.f, 0.f};
inline constexpr Vector VECTOR_ONE{1.f, 1.f};
inline constexpr Vector VECTOR_UP{0.f, -1.f};
inline constexpr Vector VECTOR_RIGHT{1.f, 0.f};
inline constexpr Vector VECTOR_DOWN{0.f, 1.f};
inline constexpr Vector VECTOR_LEFT{-1.f, 0.f};
} // namespace tnt

inline std::ostream &operator<<(std::ostream &os, tnt::Vector const &rhs) {
  os << "{" << rhs.x << ", " << rhs.y << "}";
  return os;
}

#endif //! TNT_VECTOR_HPP
