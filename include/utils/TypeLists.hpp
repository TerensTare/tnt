#ifndef TNT_TYPE_LISTS_HPP
#define TNT_TYPE_LISTS_HPP

// TypeList implementation based on
// https://monoinfinito.wordpress.com/series/introduction-to-c-template-metaprogramming/?fbclid=IwAR2cfx-NAVUFEDyG7NtSwKAhrkDE0cdIycTq39Ii91oMYoKIQ2gILbhPNq4

// TODO: use sth similar to TypeLists to create a decision making AI.
// NOTE: tl::at can be used carefully as it doesn't check for out of bounds value.

namespace tnt
{
struct NullType
{
};

template <typename H, typename T>
struct TypeList
{
    typedef H Head;
    typedef T Tail;
};

template <typename H>
struct TypeList<H, NullType>
{
    typedef H Head;
    typedef NullType Tail;
};

template <typename H, typename... T>
struct TypeList<H, TypeList<T...>>
{
    typedef H Head;
    typedef typename TypeList<T...>::Tail Tail;
};

namespace tl
{

namespace detail
{
template <typename T, typename U>
struct is_same
{
    inline static constexpr bool value = false;
};

template <typename T>
struct is_same<T, T>
{
    inline static constexpr bool value = true;
};

template <bool, typename T = void>
struct enable_if
{
};

template <typename T>
struct enable_if<true, T>
{
    typedef T type;
};

template <typename T, typename U, typename = enable_if_t<is_same<T, U>::value>>
struct is_less
{
    inline static constexpr bool value = (T < U);
};

template <typename T, T a, T b>
struct is_positive
{
    inline static constexpr bool value = ((a - b) > 0);
};

template <typename T, typename U>
inline constexpr bool is_same_v = typename is_same<T, U>::value;

template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

template <typename T, T a, T b>
inline constexpr bool is_positive_v = typename is_positive<T, a, b>::value;

} // namespace detail

////////////
// length //
////////////

template <typename T>
struct length
{
    enum
    {
        value = 1
    };
};

template <>
struct length<NullType>
{
    enum
    {
        value = 0
    };
};

template <template <typename, typename> typename List>
struct length
{
    enum
    {
        value = 1 + typename length<typename List::Tail>::value;
    };
};

////////
// at //
////////

template <
    typename List, int Index,
    typename detail::enable_if<
        detail::is_positive<
            int, length<List>::value,
            Index>::value,
        int>::type = 0>
struct at
{
    typedef typename at<typename List::Tail, Index - 1>::value value;
};

template <typename List,
          typename detail::enable_if<true, int>::type>
struct at<List, 0, (typename detail::enable_if<true, int>::type)0>
{
    typedef typename List::Head value;
};

///////////
// using //
///////////

template <typename T>
using length_v = typename length<T>::value;

template <typename List, int Index>
using at_v = typename at<List, Index>::value;
} // namespace tl
} // namespace tnt

#endif //!TNT_TYPE_LISTS_HPP