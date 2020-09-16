#ifndef TNT_TYPE_LISTS_HPP
#define TNT_TYPE_LISTS_HPP

// TypeList implementation based on
// https://monoinfinito.wordpress.com/series/introduction-to-c-template-metaprogramming/?fbclid=IwAR2cfx-NAVUFEDyG7NtSwKAhrkDE0cdIycTq39Ii91oMYoKIQ2gILbhPNq4

// TODO: use sth similar to TypeLists to create a decision making AI.
// NOTE: tl::at should be used carefully as it doesn't check for out of bounds
// value.

namespace tnt
{
    struct NullType
    {};

    template <typename H, typename T> struct TypeList
    {
        typedef H Head;
        typedef T Tail;
    };

    template <typename H> struct TypeList<H, NullType>
    {
        typedef H Head;
        typedef NullType Tail;
    };

    template <typename H, typename... T> struct TypeList<H, TypeList<T...>>
    {
        typedef H Head;
        typedef typename TypeList<T...>::Tail Tail;
    };

    namespace tl
    {
        namespace detail
        {
            template <typename T, typename U> struct is_same
            {
                inline static inline constexpr bool value = false;
            };

            template <typename T> struct is_same<T, T>
            {
                inline static inline constexpr bool value = true;
            };

            template <bool, typename T = void> struct enable_if
            {};

            template <typename T> struct enable_if<true, T>
            {
                typedef T type;
            };

            template <typename T, typename U,
                      typename = enable_if_t<is_same<T, U>::value>>
            struct is_less
            {
                inline static inline constexpr bool value = (T < U);
            };

            template <typename T, T a, T b> struct is_positive
            {
                inline static inline constexpr bool value = ((a - b) > 0);
            };

            template <typename T, typename U>
            inline constexpr bool is_same_v = typename is_same<T, U>::value;

            template <bool B, typename T = void>
            using enable_if_t = typename enable_if<B, T>::type;

            template <typename T, T a, T b>
            inline constexpr bool is_positive_v =
                typename is_positive<T, a, b>::value;

        } // namespace detail

        ////////////
        // length //
        ////////////

        template <typename T> struct length;

        template <> struct length<NullType>
        {
            enum
            {
                value = 0
            };
        };

        template <typename Head, typename Tail>
        struct length<TypeList<Head, Tail>>
        {
            enum
            {
                value = 1 + length<Tail>::value;
            };
        };

        ////////
        // at //
        ////////

        template <typename List, unsigned Index> struct at;

        template <typename Head, typename Tail>
        struct at<TypeList<Head, Tail>, 0>
        {
            typedef Head type;
        };

        template <typename Head, typename Tail, unsigned Index>
        struct at<TypeList<Head, Tail>, Index>
        {
            typedef typename at<Tail, i - 1>::type type;
        };

        template <typename List, typename T> struct index;

        template <typename T> struct index<NullType, T>
        {
            enum
            {
                value = -1
            };
        };

        template <typename T, typename Tail> struct index<TypeList<T, Tail>, T>
        {
            enum
            {
                value = 0
            };
        };

        template <typename Head, typename Tail, typename T>
        struct index<TypeList<Head, Tail>, T>
        {
            enum
            {
                value = ((index<Tail, T>::value == -1)
                             ? (-1)
                             : (1 + index<Tail, T>::value))
            };
        };

        template <typename List, typename T> struct append;

        template <> struct append<NullType, NullType>
        {
            typedef NullType type;
        };

        template <typename T> struct append<NullType, T>
        {
            typedef TypeList<T, NullType> type;
        };

        template <typename Head, typename Tail>
        struct append<NullType, TypeList<Head, Tail>>
        {
            typedef TypeList<Head, Tail> type;
        };

        template <typename Head, typename Tail, typename T>
        struct append<TypeList<Head, Tail>, T>
        {
            typedef TypeList<Head, typename append<Tail, T>::type> type;
        };

        template <typename List, typename T> struct erase;

        template <typename T> struct erase<NullType, T>
        {
            typedef NullType type;
        };

        template <typename T, typename Tail> struct erase<TypeList<T, Tail>, T>
        {
            typedef Tail type;
        };

        template <typename Head, typename Tail, typename T>
        struct erase<TypeList<Head, Tail>, T>
        {
            typedef TypeList<Head, typename erase<Tail, T>::type> type;
        };

        template <typename List, typename T> struct erase_all;

        template <typename T> struct erase_all<NullType, T>
        {
            typedef NullType type;
        };

        template <typename T, typename Tail>
        struct erase_all<TypeList<T, Tail>, T>
        {
            typedef typename erase_all<Tail, T>::type type;
        };

        template <typename Head, typename Tail, typename T>
        struct erase_all<TypeList<Head, Tail>, T>
        {
            typedef TypeList<Head, typename erase_all<Tail, T>::type> type;
        };

        template <typename List> struct unique_list;

        template <> struct unique_list<NullType>
        {
            typedef NullType type;
        };

        template <typename Head, typename Tail>
        struct unique_list<TypeList<Head, Tail>>
        {
            typedef TypeList<
                Head, typedef typename erase<
                          typedef typename unique_list<Tail>::type, Head>::type>
                type;
        };

        template <typename List, typename T, typename U> struct replace;

        template <typename T, typename U> struct replace<NullType, T, U>
        {
            typedef NullType type;
        };

        template <typename T, typename Tail, typename U>
        struct replace<TypeList<T, Tail>, T, U>
        {
            typedef TypeList<U, Tail> type;
        };

        template <typename Head, typename Tail, typename T, typename U>
        struct replace<TypeList<Head, Tail>, T, U>
        {
            typedef TypeList<Head, typename replace<Tail, T, U>::type> type;
        };
        ///////////
        // using //
        ///////////

        template <typename T> using length_v = typename length<T>::value;

        template <typename List, int Index>
        using at_t = typename at<List, Index>::type;

        template <typename List, typename T>
        using index_v = index<List, T>::value;

        template <typename List, typename T>
        using append_t = typename append<List, T>::type;

        template <typename List, typename T>
        using erase_t = typename erase<List, T>::type;

        template <typename List, typename T>
        using erase_all_t = typename erase_all<List, T>::type;

        template <typename List>
        using unique_list_t = typename unique_list<List>::type;

        template <typename List, typename T, typename U>
        using replace_t = typename replace<List, T, U>::type;
    } // namespace tl
} // namespace tnt

#endif //! TNT_TYPE_LISTS_HPP