#ifndef TNT_MATH_UTILS_HPP
#define TNT_MATH_UTILS_HPP

// TODO:
// fractions
// sine, cosine, abs, ceil, clamp (maybe) log, lerp ??
// (maybe) compile-time easings

namespace tnt
{
    template <int I>
    struct factorial
    {
        enum
        {
            value = I * factorial<I - 1>::value
        };
    };

    template <>
    struct factorial<1>
    {
        enum
        {
            value = 1
        };
    };

    template <int I>
    struct fibonacci
    {
        enum
        {
            value = fibonacci<I - 1>::value + fibonacci<I - 2>::value
        };
    };

    template <>
    struct fibonacci<2>
    {
        enum
        {
            value = 1
        };
    };

    template <>
    struct fibonacci<1>
    {
        enum
        {
            value = 1
        };
    };

    template <int Base, int Expo>
    struct power
    {
        enum
        {
            value = Base * power<Base, Expo - 1>::value
        };
    };

    template <int Base>
    struct power<Base, 0>
    {
        enum
        {
            value = 1
        };
    };

    template <int Expo>
    struct power<1, Expo>
    {
        enum
        {
            value = 1
        };
    };

    template <int Expo>
    struct power<0, Expo>
    {
        enum
        {
            value = 0
        };
    };

    template <int First, int... Rest>
    struct sum
    {
        enum
        {
            value = First + sum<Rest...>::value + 0
        };
    };

    template <int First, int Second>
    struct sum<First, Second>
    {
        enum
        {
            value = First + Second
        };
    };

    template <int First, int... Rest>
    struct product
    {
        enum
        {
            value = First * product<Rest...>::value * 1
        };
    };

    template <int First, int Second>
    struct product<First, Second>
    {
        enum
        {
            value = First * Second
        };
    };

    template <int First, int Second>
    struct modulus
    {
        enum
        {
            value = (First - (First % Second))
        };
    };

    // pretty smart, huh?
    template <int Numerator, int Denominator>
    struct Floor
    {
        static_assert(
            Denominator != 0,
            "Error: Trying to find the floor of a fraction with 0 as Denominator!!");
        enum
        {
            remain = Numerator - ((int)(Numerator / Denominator) * Denominator),
            value = ((Numerator - remain) / Denominator)
        };
    };

    template <int Numerator>
    struct Floor<Numerator, 1>
    {
        enum
        {
            value = Numerator
        };
    };

    template <int Numerator, int Denominator>
    struct Ceil
    {
        static_assert(
            (Denominator != 0),
            "Error: Trying to find the ceil of a fraction with 0 as Denominator!!");
        enum
        {
            value = typename Floor<Numerator, Denominator>::value + 1
        };
    };

    inline float constexpr operator""_pi(unsigned long long num)
    {
        return (num * 3.1415926f);
    }

    // TODO: WIP
    // template <int Angle>
    // struct sine
    // {
    //     enum
    //     {
    //         value
    //     };
    // };

    inline constexpr float PI{3.1415926f};

    inline constexpr auto RadianToDegree = [](float rad) -> float {
        return (rad * (180.0f / PI));
    };
    inline constexpr auto DegreeToRadian = [](float deg) -> float {
        return (deg * (PI / 180.0f));
    };

    template <typename T>
    inline auto clamp = [](T val, T min, T max) {
        if (val < min)
            val = min;
        if (val > max)
            val = max;
        return val;
    };

    template <class T>
    inline constexpr auto lerp = [](T a, T b, float pct)
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
    inline constexpr auto blerp =
        [](T a1, T a2, T b1, T b2,
           float pct1, float pct2) {
            return (a1 * (1 - pct1) * (1 - pct2) + a2 * pct1 * (1 - pct2) +
                    b1 * pct2 * (1 - pct1) + b2 * pct1 * pct2);
        };

    template <class T>
    inline constexpr auto bezier_curve = [](T p1, T p2,
                                            T p3, T p4, float pct) {
        return ((p1 * (1 - pct) * (1 - pct) * (1 - pct)) +
                (p2 * 3 * (1 - pct) * (1 - pct) * pct) +
                (p3 * 3 * (1 - pct) * pct * pct) + (p4 * pct * pct * pct));
    };

    template <typename T>
    inline constexpr auto step = [](T a, T b) -> float {
        return static_cast<float>(a <= b);
    };

    template <typename T>
    inline constexpr auto boxstep = [](T a, T b, T x) -> float {
        return clamp<decltype(x - a / b - a)>(x - a / b - a, 0, 1);
    };

    template <typename T>
    inline constexpr auto pulse = [](T a, T b, T x) -> float {
        return step<T>(a, x) - step<T>(b, x);
    };

    inline auto smoothstep = [](float a, float b, float x) -> float {
        if (x < a)
            return 0;
        else if (x >= b)
            return 1;
        x = boxstep<float>(a, b, x);
        return (x * x * (3 - x - x));
    };

    // (maybe): use custom power function ??
    inline const auto gamacorrect = [](float gamma, float x) -> float {
        return std::powf(x, 1 / gamma);
    };

    inline const auto bias = [](float b, float x) -> float {
        return std::powf(x, std::logf(b) / std::logf(0.5));
    };

    inline const auto gain = [](float g, float x) -> float {
        if (x < 0.5)
            return bias(1 - g, x + x) / 2;
        return 1 - bias(1 - g, 2 - x - x) / 2;
    };

    // thx Quake III Arena devs
    // https://github.com/id-Software/Quake-III-Arena/blob/dbe4ddb10315479fc00086f08e25d968b4b43c49/code/game/q_shared.h#L573
    inline constexpr auto Q_rsqrt = [](float number) -> float {
        constexpr float threehalfs = 1.5F;

        float const &x2{number * 0.5F};
        float y{number};
        long i{*(long *)&y}; // evil floating point bit level hacking
        i = 0x5f3759df - (i >> 1);
        y = *(float *)&i;
        y = y * (threehalfs - (x2 * y * y));

        return y;
    };

    inline constexpr auto Q_sqrt = [](float number) -> float {
        constexpr float threehalfs = 1.5F;

        float const &x2{number * 0.5F};
        float y{number};
        long i{*(long *)&y}; // evil floating point bit level hacking
        i = 0x5f3759df - (i >> 1);
        y = *(float *)&i;
        y = y * (threehalfs - (x2 * y * y));

        return number * y;
    };

} // namespace tnt

#endif //! TNT_MATH_UTILS_HPP