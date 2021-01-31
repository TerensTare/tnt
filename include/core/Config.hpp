#ifndef CONFIG_HPP
#define CONFIG_HPP

#ifndef DOXYGEN_SHOULD_SKIP_THIS

// clang-format off
# if defined(_MSC_VER) && !defined(__clang__)
#   ifdef TNT_BUILD
#       define TNT_API __declspec(dllexport)
#       define TNT_EXPORT
#   else
//disable warnings on extern before template instantiation
#       define TNT_API __declspec(dllimport)
#       define TNT_EXPORT extern
#   endif //!TNT_BUILD_DLL
# elif defined(__GNUC__) || (defined(__clang__) && !defined(_MSC_VER))
#   ifdef TNT_BUILD
#       define TNT_API __attribute__((visibility("default")))
#       define TNT_EXPORT
#   else
#       define TNT_API
#       define TNT_EXPORT
#   endif
# endif //!_MSC_VER
// clang-format on

#endif

#endif //!CONFIG_HPP