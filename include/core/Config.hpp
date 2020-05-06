#ifndef CONFIG_HPP
#define CONFIG_HPP

#define TNT_NODISCARD [[nodiscard]]

#define TNT_NON_COPYABLE(type)   \
    type(type const &) = delete; \
    type &operator=(type const &) = delete;

#define TNT_NON_MOVABLE(type) \
    type(type &&) = delete;   \
    type &operator=(type &&) = delete;

#if defined(DEBUG) || defined(_DEBUG)
#define TNT_DEBUG

#define dbg
#define rel if (0)
#elif defined(TNT_RELEASE)
#define dbg if (0)
#define rel
#endif // !DEBUG

#define forever for (;;)

// used to simulate the labeled loop feature that other languages have.
// https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Named_Loop
#define label(name)       \
    goto name;            \
    name##_skip : if (0); \
    name:

#define leave(name) goto name##_skip;

#if defined(_WIN32) || defined(_WIN64)

#define PATH_SEPARATOR "\\"

#ifdef _MSC_VER
#ifdef TNT_BUILD_DLL
#define TNT_API __declspec(dllexport)
#else
#define TNT_API __declspec(dllimport)
#endif //!TNT_BUILD_DLL
#endif //!_MSC_VER

#define windows
#define linux if (0)
#define mac if (0)
#define android if (0)
#define ios if (0)

#elif defined(__linux__)

// TODO(maybe): this should be sth different.
#define TNT_API

#if defined(__ANDROID__) || defined(__ANDROID_API__)
#define PATH_SEPARATOR "/"

#define windows if (0)
#define linux if (0)
#define mac if (0)
#define android
#define ios if (0)

#endif // !__ANDROID__

#define TNT_NOVTABLE

#define windows if (0)
#define linux
#define mac if (0)
#define android if (0)
#define ios if (0)

#elif defined(__APPLE__) && defined(__MACH__)

// TODO(maybe): this should be sth different.
#define TNT_API

#if TARGET_IPHONE_SIMULATOR == 1
#define PATH_SEPARATOR "/"

#define windows if (0)
#define linux if (0)
#define mac if (0)
#define android if (0)
#define ios

#elif TARGET_OS_IPHONE == 1
#define PATH_SEPARATOR "/"

#define TNT_NOVTABLE

#define windows if (0)
#define linux if (0)
#define mac if (0)
#define android if (0)
#define ios

#elif TARGET_OS_MAC == 1
#define PATH_SEPARATOR "/"

#define TNT_NOVTABLE

#define windows if (0)
#define linux if (0)
#define mac
#define android if (0)
#define ios if (0)

#endif // !TARGET_IPHONE_SIMULATOR

#else
static_assert("TnT couldn't detect running operating system!");
#endif //!_WIN32

#endif //!CONFIG_HPP