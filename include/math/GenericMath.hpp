#ifndef TNT_GENERIC_MATH_HPP
#define TNT_GENERIC_MATH_HPP

namespace tnt
{
    inline constexpr float PI{3.14159f};

    inline constexpr auto RadianToDegree = [](float rad) {
        return (rad * (180.0f / PI));
    };
    inline constexpr auto DegreeToRadian = [](float deg) {
        return (deg * (PI / 180.0f));
    };

    template <typename T>
    inline auto clamp = [](T &val, T const &min, T const &max) -> decltype(val) {
        if (val < min)
            val = min;
        if (val > max)
            val = max;
        return val;
    };

    template <class T>
    inline const auto lerp = [](T const &a, T const &b, float pct)
        -> decltype(a + (b - a) * pct) {
        return a + (b - a) * pct;
    };

    // TODO: find a way so that this returns float.
    // template<class T>
    // inline const auto inv_lerp = [](T a, T b, T res)
    // {
    // 	return ((res - a) / (b - a));
    // };

    // TODO: optimise this.
    // template<class T>
    // inline const auto remap = [](T a1, T a2, T b1, T b2, T val) {
    // 	float pct = inv_lerp<T>(a1, a2, val);
    // 	return lerp<T>(b1, b2, pct);
    // };

    template <class T>
    inline const auto blerp =
        [](T const &a1, T const &a2, T const &b1, T const &b2,
           float pct1, float pct2) {
            return (a1 * (1 - pct1) * (1 - pct2) + a2 * pct1 * (1 - pct2) +
                    b1 * pct2 * (1 - pct1) + b2 * pct1 * pct2);
        };

    template <class T>
    inline const auto bezier_curve = [](T p1, T p2, T p3, T p4, float pct) {
        return ((p1 * (1 - pct) * (1 - pct) * (1 - pct)) +
                (p2 * 3 * (1 - pct) * (1 - pct) * pct) +
                (p3 * 3 * (1 - pct) * pct * pct) + (p4 * pct * pct * pct));
    };
} // namespace tnt

#endif //!TNT_GENERIC_MATH_HPP