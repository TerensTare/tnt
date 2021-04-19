#ifndef TNT_COROUTINES_UTILITIES_HPP
#define TNT_COROUTINES_UTILITIES_HPP

#include <iterator>
#include "async/Generator.hpp"

namespace tnt
{
    namespace detail
    {
        template <typename T, typename U>
        using diff_type = decltype(std::declval<U>() - std::declval<T>());

        template <typename T, typename D>
        concept advancable_from = requires(T &t, D const d) { std::ranges::advance(t, d); };
    }

    /// @brief co_yield all the elements within the range [min_, max_) while walking with a certain step on python style.
    /// @tparam T The type of min_.
    /// @tparam U The type of max_. Same as T by default.
    /// @param min_ The first element to co_yield.
    /// @param max_ The element one past the range.
    /// @param step The number that will be added to min_ each call. Defaults to 1.
    template <std::incrementable T, std::equality_comparable_with<T> U = T>
    [[nodiscard]] tnt::generator<T> range(
        T min_, U max_, detail::diff_type<T, U> step = detail::diff_type<T, U>{1})
    {
        if constexpr (detail::advancable_from<T, detail::diff_type<T, U>>)
            for (; min_ != max_; std::ranges::advance(min_, step))
                co_yield min_;
        else if constexpr (std::integral<T> && std::integral<U>)
            for (; std::cmp_not_equal(min_, max_); min_ += step) // just in case
                co_yield min_;
        else
            for (; min_ != max_; min_ += step)
                co_yield min_;
    }
} // namespace tnt

#endif //!TNT_COROUTINES_UTILITIES_HPP