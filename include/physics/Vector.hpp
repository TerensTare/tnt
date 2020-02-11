#ifndef VECTOR_HPP
#define VECTOR_HPP

#if defined(DEBUG) || defined(_DEBUG) || defined(TNT_DEBUG)
#include <iostream>
#endif

//#if __cplusplus > 201703L
//#include <compare>
//#endif

#include "physics/Core.hpp"
#include "Easings.hpp"

// TODO:
// add noexcept at more places, if possible.
// add a ScalableVector struct, which has x, y and z coordinates, and uses z as a scale of itslef.
namespace tnt
{
struct Vector
{
	constexpr Vector(real _x = 0.0, real _y = 0.0) noexcept : x{_x}, y{_y} {}

	// template <typename T1, typename T2 = T1,
	// 		  typename = std::enable_if_t<
	// 			  std::is_convertible_v<T1, real> &&
	// 			  std::is_convertible_v<T2, real> &&
	// 			  !std::is_pointer_v<T1> &&
	// 			  !std::is_pointer_v<T2>>>
	// Vector(T1 _x = 0, T2 _y = 0)
	// 	: x{static_cast<real>(_x)},
	// 	  y{static_cast<real>(_y)} {} // TODO(experimental)

	Vector(const Vector &) = default;
	Vector(Vector &&) = default;

	real MagnitudeSqr() const noexcept { return x * x + y * y; }
	real Magnitude() const noexcept { return sqrt(x * x + y * y); }

	void Normalize() noexcept
	{
		real mag{Magnitude()};
		x /= mag;
		y /= mag;
	}

	Vector Normalized() const
	{
		real mag{Magnitude()};
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

	bool operator<(const Vector &rhs) const noexcept { return std::tie(x, y) < std::tie(rhs.x, rhs.y); }
	bool operator>(const Vector &rhs) const noexcept { return ((x > rhs.x) && (y > rhs.y)); }

	//#if __cplusplus > 201703L // operator<=> isn't available before C++20
	//		auto operator<=>(Vector const&) const = default;
	//#endif

	friend std::ostream &operator<<(std::ostream &os, const Vector &rhs);

	real x;
	real y;
};

inline Vector operator+(const Vector &lhs, const Vector &rhs) { return Vector{lhs.x + rhs.x, lhs.y + rhs.y}; }
inline Vector operator-(const Vector &lhs, const Vector &rhs) { return Vector{lhs.x - rhs.x, lhs.y - rhs.y}; }
inline Vector operator*(const Vector &vec, const real &num) { return Vector{vec.x * num, vec.y * num}; }
inline Vector operator/(const Vector &vec, const real &num) { return Vector{vec.x / num, vec.y / num}; }

inline real Dot(const Vector &lhs, const Vector &rhs)
{
	real angleCos = static_cast<real>((lhs.x * rhs.x + lhs.y * rhs.y) / (lhs.Magnitude() * rhs.Magnitude()));
	real angle = convert::RadianToDegree(acosf(angleCos));
	return angle;
}

inline real Cross(Vector const &lhs, Vector const &rhs)
{
	return ((lhs.x * rhs.y) - (lhs.y * rhs.x));
}

inline Vector RotateVector(Vector &vec, real angle)
{
	real radAngle = static_cast<real>(convert::DegreeToRadian(angle));
	return Vector{
		static_cast<real>(vec.x * cosf(radAngle) - vec.y * sinf(radAngle)),
		static_cast<real>(vec.x * sinf(radAngle) + vec.y * cosf(radAngle))};
}

inline Vector nlerp(Vector left, Vector right, real pct)
{
	return lerp<Vector>(left, right, pct).Normalized();
}

inline bool operator<=(const Vector &lhs, const Vector &rhs) noexcept { return ((lhs.x <= rhs.x) && (lhs.y <= rhs.y)); }
inline bool operator>=(const Vector &lhs, const Vector &rhs) noexcept { return ((lhs.x >= rhs.x) && (lhs.y >= rhs.y)); }
inline bool operator==(const Vector &lhs, const Vector &rhs) noexcept { return ((lhs.x == rhs.x) && (lhs.y == rhs.y)); }

#if __cplusplus <= 201703L || defined(TNT_OPERATOR_NEQUAL) || defined(TNT_ALLOW_DEPRECATIONS) // operator!= is deprecated in C++20
inline bool operator!=(const Vector &lhs, const Vector &rhs) noexcept
{
	return ((lhs.x != rhs.x) || (lhs.y != rhs.y));
}
#endif
} // namespace tnt

inline std::ostream &tnt::operator<<(std::ostream &os, const Vector &rhs)
{
	os << "{" << rhs.x << ", " << rhs.y << "}";
	return os;
}

#endif //!VECTOR_HPP