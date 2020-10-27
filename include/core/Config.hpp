#ifndef CONFIG_HPP
#define CONFIG_HPP

#if defined(_MSC_VER) && !defined(__clang__)
// class X needs to have dll-interface to be used by clients of class Y
#pragma warning(disable : 4251)
#endif

#if defined(_MSC_VER) && !defined(__clang__)
#ifdef TNT_BUILD
#define TNT_API __declspec(dllexport)
#else
#define TNT_API __declspec(dllimport)
#endif //!TNT_BUILD_DLL
#elif defined(__GNUC__) || (defined(__clang__) && !defined(_MSC_VER))
#ifdef TNT_BUILD
#define TNT_API __attribute__((visibility("default")))
#else
#define TNT_API
#endif
#endif //!_MSC_VER

#endif //!CONFIG_HPP