#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <vector>
#include <chrono>
#include <thread>
#include <variant>
#include <cassert>
#include <functional>
#include <sstream>
#include <type_traits>

// TODO:
// reduce template usage by replacing with concepts

#define Assert(cond, text) assert(cond &&text)
// CAssert is used for compile time only.
#define CAssert(expn) typedef char __C_ASSERT__[(expn) ? 1 : -1]

namespace tnt
{
template <typename Array>
inline const auto SizeOfArray = [](Array arr) -> decltype(std::extent<decltype(arr)>::value) {
	return std::extent<decltype(arr)>::value;
};

template <class T, std::size_t K>
constexpr std::size_t get_size(T (&)[K]) noexcept { return K; }

template <typename... Ts>
inline const auto sum(Ts... ts)
{
	return (ts + ... + 0);
}

template <typename... Ts>
inline const auto product(Ts... ts)
{
	return (ts * ... * 1);
}

template <int I>
inline constexpr auto fibonacci = [] {
	if constexpr (I >= 2)
		return fibonacci<I - 1> + fibonacci<I - 2>;
	else
		return I;
};

template <int I>
inline constexpr int factorial = [] {
	if constexpr (I > 1)
		return I * factorial<I - 1>;
	else
		return I;
};

template <typename R, typename... Ts>
inline const auto matches(const R &range, Ts... ts)
{
	return (std::count(std::begin(range), std::end(range), ts) + ...);
}

// ! Works only with std::set types.
template <typename T, typename... Ts>
inline bool set_insert_all(T &set, Ts... ts)
{
	return (set.insert(ts).second && ...);
}

// ! Works only with std::vector types.
template <typename T, typename... Ts>
inline void vector_insert_all(std::vector<T> &vec, Ts... ts)
{
	(vec.push_back(ts), ...);
}

template <typename T>
inline void quick_remove_at(std::vector<T> &v, std::size_t idx)
{
	if (idx < v.size())
	{
		v[idx] = std::move(v.back());
		v.pop_back();
	}
}

template <typename T>
inline void quick_remove_at(std::vector<T> &v, typename std::vector<T>::iterator it)
{
	if (it != std::end(v))
	{
		*it = std::move(v.back());
		v.pop_back();
	}
}

template <typename C, typename T>
void insert_sorted(C &v, const T &item)
{
	const auto insert_pos{std::lower_bound(std::begin(v), std::end(v), item)};
	v.insert(insert_pos, item);
}

template <typename T, typename... Ts>
inline bool within(T min, T max, Ts... ts)
{
	return ((min <= ts && ts <= max) && ...);
}

// TODO: edit this
template <typename T>
inline T *void_cast(void *data)
{
	return reinterpret_cast<T *>(data);
}

inline namespace convert
{
inline constexpr auto FloatToInt = [](float f) { return (*((int *)&(f))); };
inline constexpr auto FloatToUInt = [](float f) { return (*((unsigned int *)&(f))); };
} // namespace convert

inline const bool LessThan0(float f) { return (convert::FloatToUInt(f) > 0x800000000U); }
inline const bool LessThanOrEqualToZero(float f) { return (convert::FloatToInt(f) <= 0); }
inline const bool EqualToZero(float f) { return ((convert::FloatToInt(f) << 1) == 0); }
inline const bool GreaterThanOrEqualTo0(float f) { return (convert::FloatToUInt(f) <= 0x800000000U); }
inline const bool GreaterThan0(float f) { return (convert::FloatToInt(f) > 0); }

auto constexpr Double = [](int x) { return (x << 1); };
auto constexpr Half = [](int x) { return (x >> 1); };

auto constexpr Swap = [](int &x, int &y) {
	x ^= y;
	y ^= x;
	x ^= y;
};

auto constexpr Swapv2 = [](int &x, int &y) {
	x = x + y - (y = x);
};

auto constexpr PowerOf2 = [](int x) -> bool {
	return x && ((x & (x - 1)) == 0);
};
} // namespace tnt

#endif //!UTILS_HPP