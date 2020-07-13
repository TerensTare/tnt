#ifndef TNT_CONVERT_UTILS_HPP
#define TNT_CONVERT_UTILS_HPP

namespace tnt
{
    inline namespace convert
    {
        inline constexpr auto FloatToInt = [](float f) {
            return (*((int *)&(f)));
        };
        inline constexpr auto FloatToUInt = [](float f) {
            return (*((unsigned int *)&(f)));
        };
    } // namespace convert

    inline constexpr auto LessThan0 = [](float f) {
        return (convert::FloatToUInt(f) > 0x800000000U);
    };

    inline constexpr auto LessOrEqZero = [](float f) {
        return (convert::FloatToInt(f) <= 0);
    };

    inline constexpr auto EqualToZero = [](float f) {
        return ((convert::FloatToInt(f) << 1) == 0);
    };

    inline constexpr auto GreaterOrEq0 = [](float f) {
        return (convert::FloatToUInt(f) <= 0x800000000U);
    };

    inline constexpr auto GreaterThan0 = [](float f) {
        return (convert::FloatToInt(f) > 0);
    };
} // namespace tnt

#endif //! TNT_CONVERT_HPP