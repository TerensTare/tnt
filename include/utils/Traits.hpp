#ifndef TNT_TYPE_TRAITS_UTILITIES_HPP
#define TNT_TYPE_TRAITS_UTILITIES_HPP

// TODO:
// support more os and compiler checks.

namespace tnt
{
        namespace detail
        {
                // void_t
                template <typename... Ts>
                using void_t = void;

                // is_same_v
                template <typename T, typename U>
                inline constexpr bool is_same_v{false};

                template <typename T>
                inline constexpr bool is_same_v<T, T>{true};

                // is_any_of
                template <typename T, typename... Ts>
                inline constexpr bool is_any_of_v{(is_same_v<T, Ts> || ...)};
        } // namespace detail

        // thx Jonathan Boccara
        // https://www.fluentcpp.com/2020/05/29/how-to-make-derived-classes-implement-more-than-assignment/
        template <template <typename...> class Expression, typename Attempt, typename... Ts>
        struct is_detected
        {
                inline constexpr bool value{false};
        };

        template <template <typename...> class Expression, typename... Ts>
        struct is_detected<Expression, detail::void_t<Expression<Ts...>>, Ts...>
        {
                inline constexpr bool value{true};
        };

        template <template <typename...> class Expression, typename... Ts>
        inline constexpr bool is_detected_v = is_detected<Expression, void, Ts...>::value;

        /// @brief A helper struct for tag-like types.
        struct tag_type_t
        {
                tag_type_t() = delete;
                ~tag_type_t() = delete;

                tag_type_t(tag_type_t const &) = delete;
                tag_type_t &operator=(tag_type_t const &) = delete;

                tag_type_t(tag_type_t &&) = delete;
                tag_type_t &operator=(tag_type_t &&) = delete;
        };

        struct windows_t final : tag_type_t
        {
        };

        struct linux_t final : tag_type_t
        {
        };

        struct macos_t final : tag_type_t
        {
        };

        struct ios_t final : tag_type_t
        {
        };

        struct android_t final : tag_type_t
        {
        };

        struct unix_t final : tag_type_t
        {
        };

        template <typename T>
        concept os = detail::is_any_of_v<T, windows_t, linux_t, macos_t, ios_t, android_t, unix_t>;

        template <os O>
        struct is_os;

        template <>
        struct is_os<windows_t>
        {
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
                inline constexpr bool value{true};
#else
                inline constexpr bool value{false};
#endif
        };

        template <>
        struct is_os<linux_t>
        {
#if defined(__linux__) && !defined(__android__)
                inline constexpr bool value{true};
#else
                inline constexpr bool value{false};
#endif
        };

        template <>
        struct is_os<macos_t>
        {
#ifdef TARGET_OS_MAC
                inline constexpr bool value{true};
#else
                inline constexpr bool value{false};
#endif
        };

        template <>
        struct is_os<ios_t>
        {
#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
                inline constexpr bool value{true};
#else
                inline constexpr bool value{false};
#endif
        };

        template <>
        struct is_os<android_t>
        {
#ifdef __android__
                inline constexpr bool value{true};
#else
                inline constexpr bool value{false};
#endif
        };

        template <>
        struct is_os<unix_t>
        {
#if !defined(__linux__) && defined(__unix__)
                inline constexpr bool value{true};
#else
                inline constexpr bool value{false};
#endif
        };

        template <os O>
        inline constexpr bool is_os_v{is_os<O>::value};

        inline constexpr bool is_windows_v{is_os<windows_t>::value};
        inline constexpr bool is_linux_v{is_os<linux_t>::value};
        inline constexpr bool is_macos_v{is_os<macos_t>::value};
        inline constexpr bool is_ios_v{is_os<ios_t>::value};
        inline constexpr bool is_android_v{is_os<android_t>::value};
        inline constexpr bool is_unix_v{is_os<unix_t>::value};

        struct gcc_t final : tag_type_t
        {
        };

        struct clang_t final : tag_type_t
        {
        };

        struct msvc_t final : tag_type_t
        {
        };

        template <typename T>
        concept compiler = detail::is_any_of_v<T, gcc_t, clang_t, msvc_t>;

        template <compiler C>
        struct is_compiler;

        template <>
        struct is_compiler<gcc_t>
        {
#ifdef __GNUC__
                inline constexpr bool value{true};
#else
                inline constexpr bool value{false};
#endif
        };

        template <>
        struct is_compiler<clang_t>
        {
#ifdef __clang__
                inline constexpr bool value{true};
#else
                inline constexpr bool value{false};
#endif
        };

        template <>
        struct is_compiler<msvc_t>
        {
#ifdef _MSC_VER
                inline constexpr bool value{true};
#else
                inline constexpr bool value{false};
#endif
        };

        template <compiler C>
        inline constexpr bool is_compiler_v{is_compiler<C>::value};

        inline constexpr bool is_gcc_v{is_compiler<gcc_t>::value};
        inline constexpr bool is_clang_v{is_compiler<clang_t>::value};
        inline constexpr bool is_msvc_v{is_compiler<msvc_t>::value};
} // namespace tnt

#endif //!TNT_TYPE_TRAITS_UTILITIES_HPP