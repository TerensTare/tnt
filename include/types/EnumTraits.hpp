#ifndef TNT_ENUMERATION_TRAITS_HPP
#define TNT_ENUMERATION_TRAITS_HPP

#include <type_traits>

namespace tnt
{
    template <typename T>
    struct enum_traits
    {
        static_assert(
            requires { T::count; },
            "Enum doesn't provide value named count!! Please specialise tnt::enum_traits<T>::count"
            " for your enum by setting it to the greatest named value + 1 or provide a value in the"
            " enum named count after the last value.");

        inline static constexpr T count = T::count;    ///< Value signalising TnT's stringifier functions to stop parsing values.
        inline static constexpr bool jsonable = false; ///< Value indicating whether TnT should automatically generate json bindings for the enum, defaulting to false.
        inline static constexpr bool luable = false;   ///< TBD
    };
} // namespace tnt

#endif //!TNT_ENUMERATION_TRAITS_HPP