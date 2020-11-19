#ifndef TNT_TYPE_LISTS_HPP
#define TNT_TYPE_LISTS_HPP

// TypeList implementation based on
// https://monoinfinito.wordpress.com/series/introduction-to-c-template-metaprogramming/?fbclid=IwAR2cfx-NAVUFEDyG7NtSwKAhrkDE0cdIycTq39Ii91oMYoKIQ2gILbhPNq4

// TODO: use sth similar to TypeLists to create a decision making AI.
// NOTE: tl::at should be used carefully as it doesn't check for out of bounds
// value.

namespace tnt
{
    template <typename...>
    struct type_list
    {
    };

    namespace tl
    {
        ////////////
        // length //
        ////////////

        template <typename T>
        struct length;

        template <>
        struct length<type_list<>>
        {
            inline static constexpr size_t value{0};
        };

        template <typename T, typename... Ts>
        struct length<type_list<T, Ts...>>
        {
            inline static constexpr size_t value{1 + length<type_list<Ts...>>::value};
        };

        ////////
        // at //
        ////////

        template <typename List, size_t Index>
        struct at;

        template <std::size_t Index>
        struct at<type_list<>, Index>
        {
            using type = type_list<>;
        };

        template <typename T, typename... Ts>
        struct at<type_list<T, Ts...>, 0>
        {
            using type = T;
        };

        template <typename T, typename... Ts, size_t Index>
        struct at<type_list<T, Ts...>, Index>
        {
            using type = typename at<type_list<Ts...>, Index - 1>::type;
        };

        template <typename List, typename T>
        struct index;

        template <typename T, typename... Ts>
        struct index<type_list<T, Ts...>, T>
        {
            inline static constexpr size_t value{0};
        };

        template <typename T, typename... Ts, typename F>
        struct index<type_list<T, Ts...>, F>
        {
            inline static constexpr size_t value{1 + append<type_list<Ts...>, F>::value};
        };

        template <typename List, typename T>
        struct append;

        template <typename... Ts, typename T>
        struct append<type_list<Ts...>, T>
        {
            using type = type_list<Ts..., T>;
        };

        template <typename List, typename T>
        struct erase;

        template <typename T>
        struct erase<type_list<>, T>
        {
            using type = type_list<>;
        };

        template <typename T, typename... Ts>
        struct erase<type_list<T, Ts...>, T>
        {
            using type = type_list<Ts...>;
        };

        template <typename T, typename... Ts, typename E>
        struct erase<type_list<T, Ts...>, E>
        {
            using type = type_list<T, typename erase<type_list<Ts...>, E>::value>;
        };

        template <typename List, typename T>
        struct erase_all;

        template <typename T>
        struct erase_all<type_list<>, T>
        {
            using type = type_list<>;
        };

        template <typename T, typename... Ts>
        struct erase_all<type_list<T, Ts...>, T>
        {
            using type = typename erase_all<type_list<Ts...>, T>::type;
        };

        template <typename T, typename... Ts, typename E>
        struct erase_all<type_list<T, Ts...>, E>
        {
            using type = type_list<T, typename erase_all<type_list<Ts...>, E>::type>;
        };

        template <typename List, typename T, typename U>
        struct replace;

        template <typename T, typename... Ts, typename U>
        struct replace<type_list<T, Ts...>, T, U>
        {
            using type = type_list<U, Ts...>;
        };

        template <typename T, typename... Ts, typename O, typename N>
        struct replace<type_list<T, Ts...>, O, N>
        {
            using type = type_list<T, typename replace<type_list<Ts...>, O, N>::type>;
        };

        ///////////
        // using //
        ///////////

        template <typename T>
        inline constexpr size_t length_v{length<T>::value};

        template <typename List, size_t Index>
        using at_t = typename at<List, Index>::type;

        template <typename List, typename T>
        inline constexpr size_t index_v = index<List, T>::value;

        template <typename List, typename T>
        using append_t = typename append<List, T>::type;

        template <typename List, typename T>
        using erase_t = typename erase<List, T>::type;

        template <typename List, typename T>
        using erase_all_t = typename erase_all<List, T>::type;

        template <typename List, typename T, typename U>
        using replace_t = typename replace<List, T, U>::type;
    } // namespace tl
} // namespace tnt

#endif //! TNT_TYPE_LISTS_HPP