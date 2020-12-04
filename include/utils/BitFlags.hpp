#ifndef TNT_UTILS_ENUM_CLASS_BIT_FLAGS_HPP
#define TNT_UTILS_ENUM_CLASS_BIT_FLAGS_HPP

#include <type_traits>

namespace tnt
{
    // thx Anthony Williams for all of the content of this header.
    // https://blog.bitwigglers.org/using-enum-classes-as-type-safe-bitmasks/

    template <typename T>
    concept enum_type = std::is_enum_v<T>;

    template <enum_type E>
    struct enable_bit_mask : std::false_type
    {
    };

    template <typename T>
    concept bit_mask = enum_type<T> &&enable_bit_mask<T>::value == true;

    template <bit_mask E>
    constexpr bool has_flag(E const &bit, E const &flag) noexcept
    {
        return (bit & flag) == flag;
    }

    template <bit_mask E>
    constexpr void set_flag(E &bit, E const &flag) noexcept
    {
        if constexpr (!has_flag(bit, flag))
            bit |= flag;
    }

    template <bit_mask E>
    constexpr void unset_flag(E &bit, E const &flag) noexcept
    {
        if constexpr (has_flag(bit, flag))
            bit &= ~flag;
    }
} // namespace tnt

template <tnt::bit_mask E>
constexpr E operator|(E const &a, E const &b)
{
    using type = std::underlying_type_t<E>;
    return static_cast<E>(
        static_cast<type>(a) | static_cast<type>(b));
}

template <tnt::bit_mask E>
constexpr E operator&(E const &a, E const &b)
{
    using type = std::underlying_type_t<E>;
    return static_cast<E>(
        static_cast<type>(a) & static_cast<type>(b));
}

template <tnt::bit_mask E>
constexpr E operator^(E const &a, E const &b)
{
    using type = std::underlying_type_t<E>;
    return static_cast<E>(
        static_cast<type>(a) ^ static_cast<type>(b));
}

template <tnt::bit_mask E>
constexpr E operator~(E const &a)
{
    using type = std::underlying_type_t<E>;
    return static_cast<E>(~static_cast<type>(a));
}

template <tnt::bit_mask E>
constexpr E &operator|=(E &a, E const &b)
{
    using type = std::underlying_type_t<E>;
    a = static_cast<E>(
        static_cast<type>(a) | static_cast<type>(b));
    return a;
}

template <tnt::bit_mask E>
constexpr E &operator&=(E &a, E const &b)
{
    using type = std::underlying_type_t<E>;
    a = static_cast<E>(
        static_cast<type>(a) & static_cast<type>(b));
    return a;
}

template <tnt::bit_mask E>
constexpr E &operator^=(E &a, E const &b)
{
    using type = std::underlying_type_t<E>;
    a = static_cast<E>(
        static_cast<type>(a) ^ static_cast<type>(b));
    return a;
}

#endif //!TNT_UTILS_ENUM_CLASS_BIT_FLAGS_HPP