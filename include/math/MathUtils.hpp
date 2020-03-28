#ifndef TNT_MATH_UTILS_HPP
#define TNT_MATH_UTILS_HPP

// TODO:
// fractions
// sine, cosine, abs, ceil, clamp (maybe) log, lerp ??

namespace tnt
{
template <int I>
struct factorial
{
    enum
    {
        value = I * factorial<I - 1>::value;
    }
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
        value = Base * power<Base, Expo - 1>::value;
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
    static_assert((Denominator != 0), "Error: Trying to find the floor of a fraction with 0 as Denominator!!");
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
struct ceiling
{
    static_assert((Denominator != 0), "Error: Trying to find the floor of a fraction with 0 as Denominator!!");
    enum
    {
        value = typename Floor<Numerator, Denominator>::value + 1
    };
};

template <int Angle>
struct sine
{
    enum
    {
        value
    };
};
} // namespace tnt

#endif //!TNT_MATH_UTILS_HPP