#ifndef CONFIG_HPP
#define CONFIG_HPP

// clang-format off
# if defined(_MSC_VER) && !defined(__clang__)
#   ifdef TNT_BUILD
#       define TNT_API __declspec(dllexport)
#   else
#       define TNT_API __declspec(dllimport)
#   endif //!TNT_BUILD_DLL
# elif defined(__GNUC__) || (defined(__clang__) && !defined(_MSC_VER))
#   ifdef TNT_BUILD
#       define TNT_API __attribute__((visibility("default")))
#   else
#       define TNT_API
#   endif
# endif //!_MSC_VER
// clang-format on

#endif //!CONFIG_HPP