#ifndef TNT_COROUTINES_UTILITIES_HPP
#define TNT_COROUTINES_UTILITIES_HPP

#include <iterator>
#include "async/Generator.hpp"

// clang-format off
namespace tnt
{
    /// @brief co_yield all the elements within the range [min_, max_) while walking with a certain step on python style.
    /// @tparam T The type of min_.
    /// @tparam U The type of max_. Same as T by default.
    /// @param min_ The first element to co_yield.
    /// @param max_ The element one past the range.
    /// @param step The number that will be added to min_ each call. Defaults to 1.
    template <std::incrementable T, typename U = T>
    requires std::equality_comparable_with<T, U>
    [[nodiscard]] tnt::generator<T> range(T min_, U max_,
        decltype(std::declval<U>() - std::declval<T>()) step =
            decltype(std::declval<U>() - std::declval<T>()){1})
    {
        for (; min_ != max_; min_ += step)
            co_yield min_;
    }
} // namespace tnt
// clang-format on

#endif //!TNT_COROUTINES_UTILITIES_HPP