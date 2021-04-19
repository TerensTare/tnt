#ifndef TNT_MIRROR_LIGHT_HPP
#define TNT_MIRROR_LIGHT_HPP

#include <array>
#include <string_view>

#include "types/EnumTraits.hpp"

namespace tnt
{
    /// @brief Get the string representation of a type.
    /// @tparam T The desired type.
    template <typename T>
    [[nodiscard]] constexpr std::string_view type_name() noexcept
    {
        // thx Boost.UT authors
        // https://github.com/boost-ext/ut/blob/3b05dca6a629497910cf8e92aebcaead0124c8b4/include/boost/ut.hpp#L228
#if defined(_MSC_VER) and not defined(__clang__)
        return {__FUNCSIG__ + 89, sizeof(__FUNCSIG__) - 106};
#elif defined(__clang__)
        return {__PRETTY_FUNCTION__ + 39, sizeof(__PRETTY_FUNCTION__) - 41};
#elif defined(__GNUC__)
        return {__PRETTY_FUNCTION__ + 54, sizeof(__PRETTY_FUNCTION__) - 105};
#endif
    }

    /// @brief Get the string representation of a variable.
    /// @tparam A The desired variable.
    template <auto A>
    [[nodiscard]] constexpr std::string_view value_name() noexcept
    {
        // thx Boost.UT authors
        // https://github.com/boost-ext/ut/blob/3b05dca6a629497910cf8e92aebcaead0124c8b4/include/boost/ut.hpp#L228
#if defined(_MSC_VER) and not defined(__clang__)
        return {__FUNCSIG__ + 89, sizeof(__FUNCSIG__) - 106};
#elif defined(__clang__)
        return {__PRETTY_FUNCTION__ + 40, sizeof(__PRETTY_FUNCTION__) - 42};
#elif defined(__GNUC__)
        return {__PRETTY_FUNCTION__ + 60, sizeof(__PRETTY_FUNCTION__) - 111};
#endif
    }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    namespace detail
    {
        template <auto A>
        requires std::is_enum_v<decltype(A)> [[nodiscard]] constexpr std::string_view enum_name() noexcept
        {
            // scoped enums
            if constexpr (not std::convertible_to<decltype(A), std::underlying_type_t<decltype(A)>>)
            {
                constexpr std::string_view name{value_name<A>()};
                return name.substr(name.find(':') + 2, name.size() - name.rfind(':'));
            }
            else
                return value_name<A>();
        }

        inline constexpr auto valid = [](char c) noexcept -> std::size_t {
            return not((c >= '0' and c <= '9') or c == '(');
        };

        template <typename T, std::size_t HintMax>
        inline constexpr std::size_t count = []<std::size_t... S>(std::index_sequence<S...>) noexcept->std::size_t
        {
            return (valid(value_name<static_cast<T>(S)>()[0]) + ... + 0u);
        }
        (std::make_index_sequence<HintMax>{});

        inline constexpr auto enum_loop = []<typename Arr, std::size_t... S>(Arr & arr, std::size_t &i, std::index_sequence<S...>) noexcept
        {
            using T = decltype(std::declval<typename Arr::value_type>().second);
            (((detail::valid(value_name<static_cast<T>(S)>()[0]))
                  ? (arr[i++] = std::pair{detail::enum_name<static_cast<T>(S)>(), static_cast<T>(S)}, (void)0)
                  : (void)0),
             ...);
            return arr;
        };
    }
#endif

    // clang-format off
    /// @brief Get an array with the names of the entries of a desired enum.
    /// @tparam T The desired enum.
    /// @tparam last The enum entry serving as a "sentinel" value, telling the function to stop "parsing".
    /// @note `last` will not be parsed by the function.
    template <typename T, T last = enum_traits<T>::count>
        requires std::is_enum_v<std::remove_cvref_t<T>> 
    [[nodiscard]] constexpr auto enums() noexcept
        -> std::array<std::pair<std::string_view, T>, detail::count<T, static_cast<std::size_t>(last)>>
    {
        // clang-format on
        using type = std::underlying_type_t<T>;
        std::array<std::pair<std::string_view, T>, detail::count<T, static_cast<std::size_t>(last)>> arr{};
        std::size_t i{};

        return detail::enum_loop(arr, i, std::make_index_sequence<static_cast<type>(last)>{});
    }

    // clang-format off
    /// @brief Get the string representation of an enum value.
    /// @tparam T The type of the enum.
    /// @param value The value you want to stringify.
    template <typename T>
        requires std::is_enum_v<T>
    [[nodiscard]] constexpr auto enum_name(T const& value) noexcept
    {
        for (constexpr auto lookup = enums<T>();
            auto const& [k, v] : lookup)
                if (value == v)
                    return k;

        using namespace std::string_view_literals;
        return ""sv;
    }
    // clang-format on

    // clang-format off
    /// @brief Get an enum value from a string representation.
    /// @tparam T The type of the enum.
    /// @param value The string from which you want to get the value.
    template <typename T>
        requires std::is_enum_v<T>
    [[nodiscard]] constexpr auto enum_value(std::string_view name) noexcept
    {
        for (constexpr auto lookup = enums<T>();
            auto const& [k, v] : lookup)
                if (name == k)
                    return v;
        return T{};
    }
    // clang-format on
} // namespace tnt

#endif //!TNT_MIRROR_LIGHT_HPP