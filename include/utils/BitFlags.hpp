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
} // namespace tnt

template <tnt::bit_mask E>
inline constexpr E operator|(E const &a, E const &b)
{
    using type = std::underlying_type_t<E>;
    return static_cast<E>(
        static_cast<type>(a) | static_cast<type>(b));
}

template <tnt::bit_mask E>
inline constexpr E operator&(E const &a, E const &b)
{
    using type = std::underlying_type_t<E>;
    return static_cast<E>(
        static_cast<type>(a) & static_cast<type>(b));
}

template <tnt::bit_mask E>
inline constexpr E operator^(E const &a, E const &b)
{
    using type = std::underlying_type_t<E>;
    return static_cast<E>(
        static_cast<type>(a) ^ static_cast<type>(b));
}

template <tnt::bit_mask E>
inline constexpr E operator~(E const &a)
{
    using type = std::underlying_type_t<E>;
    return static_cast<E>(~static_cast<type>(a));
}

template <tnt::bit_mask E>
inline constexpr E &operator|=(E &a, E const &b)
{
    using type = std::underlying_type_t<E>;
    return static_cast<E>(
        static_cast<type>(a) |= static_cast<type>(b));
}

template <tnt::bit_mask E>
inline constexpr E &operator&=(E &a, E const &b)
{
    using type = std::underlying_type_t<E>;
    return static_cast<E>(
        static_cast<type>(a) &= static_cast<type>(b));
}

template <tnt::bit_mask E>
inline constexpr E &operator^=(E &a, E const &b)
{
    using type = std::underlying_type_t<E>;
    return static_cast<E>(
        static_cast<type>(a) ^= static_cast<type>(b));
}

#endif //!TNT_UTILS_ENUM_CLASS_BIT_FLAGS_HPP