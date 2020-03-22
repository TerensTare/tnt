#ifndef TNT_MATH_UTILS_HPP
#define TNT_MATH_UTILS_HPP

// TODO:
// sine, cosine

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
        value = First + Second + 0
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
        value = First + Second
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