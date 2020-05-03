#ifndef TNT_LOGGER_HPP
#define TNT_LOGGER_HPP

// NOTE: most of these #if* preprocessors will be removed later.

#if __cplusplus >= 201704L
export module tnt : logger; // just for preparations
#define EXPORT export;
#else
#define EXPORT
#endif

#include <fmt/core.h>

// TODO: unfinished
// TODO: make colored output, and maybe write to html/xml files for better output.
// TODO: temporary use libfmt, but after MSVC will support C++20's <format>, use that instead.
// TODO: support writing to files, maybe by using fmt::vprint.
// TODO: support writing logs to the developer console built in the engine.
// TODO: make sure that tnt::logger::debug writes sth only if DEBUG || _DEBUG is defined.
// TODO: correctly quit the application when tnt::logger::error is called and you are in debug mode.
// TODO: user defined literals for any logging type.
// && ex. "Initialized subsystem x"_info.

// TODO(maybe): make this an extension.
// TODO(maybe): add macro TNT_TERMINATE_ON_ERROR.

// Priorities are in ascending order:
namespace tnt::logger
{
namespace
{
template <typename T>
EXPORT void trace(std::string_view format, T data)
{
    fmt::print("{} TRACE: {}\n", __TIME__,
               fmt::format(format, std::forward<T>(data)));
}

template <typename... Args>
EXPORT void trace(std::string_view format, Args &&... args)
{
    fmt::print("{} TRACE: {}\n", __TIME__,
               fmt::format(format, std::forward<Args>(args)...));
}

template <>
EXPORT void trace(std::string_view format)
{
    fmt::print("{} TRACE: {}\n", __TIME__, format);
}

template <typename T>
EXPORT void debug(std::string_view format, T data)
{
    fmt::print("{} DEBUG: {}\n", __TIME__,
               fmt::format(format, std::forward<T>(data)));
}

template <typename... Args>
EXPORT void debug(std::string_view format, Args &&... args)
{
    fmt::print("{} DEBUG: {}\n", __TIME__,
               fmt::format(format, std::forward<Args>(args)...));
}

template <>
EXPORT void debug(std::string_view format)
{
    fmt::print("{} DEBUG: {}\n", __TIME__, format);
}

template <typename T>
EXPORT void info(std::string_view format, T data)
{
    fmt::print("{} INFO: {}\n", __TIME__,
               fmt::format(format, std::forward<T>(data)));
}

template <class... Args>
EXPORT void info(std::string_view format, Args &&... args)
{
    fmt::print("{} INFO: {}\n", __TIME__,
               fmt::format(format, std::forward<Args>(args)...));
}

template <>
EXPORT void info(std::string_view format)
{
    fmt::print("{} INFO: {}\n", __TIME__, format);
}

template <typename T>
EXPORT void notice(std::string_view format, T data)
{
    fmt::print("{} NOTICE: {}\n", __TIME__,
               fmt::format(format, std::forward<T>(data)));
}

template <typename... Args>
EXPORT void notice(std::string_view format, Args &&... args)
{
    fmt::print("{} NOTICE: {}\n", __TIME__,
               fmt::format(format, std::forward<Args>(args)...));
}

template <>
EXPORT void notice(std::string_view format)
{
    fmt::print("{} NOTICE: {}\n", __TIME__, format);
}

template <typename T>
EXPORT void warning(std::string_view format, T data)
{
    fmt::print("{} WARNING: {}\n", __TIME__,
               fmt::format(format, std::forward<T>(data)));
}

template <typename... Args>
EXPORT void warning(std::string_view format, Args &&... args)
{
    fmt::print("{} WARNING: {}\n", __TIME__,
               fmt::format(format, std::forward<Args>(args)...));
}

template <>
EXPORT void warning(std::string_view format)
{
    fmt::print("{} WARNING: {}\n", __TIME__, format);
}

template <typename T>
EXPORT void error(std::string_view format, T data)
{
    fmt::print("{} ERROR: {}\n", __TIME__,
               fmt::format(format, std::forward<T>(data)));
    std::quick_exit(-1);
}

template <typename... Args>
EXPORT void error(std::string_view format, Args &&... args)
{
    fmt::print("{} ERROR: {}\n", __TIME__,
               fmt::format(format, std::forward<Args>(args)...));
    std::quick_exit(-1);
}

template <>
EXPORT void error(std::string_view format)
{
    fmt::print("{} ERROR: {}\n", __TIME__, format);
    std::quick_exit(-1);
}

template <typename T>
EXPORT void critical(std::string_view format, T data)
{
    fmt::print("{} CRITICAL: {}\n", __TIME__,
               fmt::format(format, std::forward<T>(data)));
}

template <typename... Args>
EXPORT void critical(std::string_view format, Args &&... args)
{
    fmt::print("{} CRITICAL: {}\n", __TIME__,
               fmt::format(format, std::forward<Args>(args)...));
}

template <>
EXPORT void critical(std::string_view format)
{
    fmt::print("{} CRITICAL: {}\n", __TIME__, format);
}
} // namespace
} // namespace tnt::logger

#endif //!TNT_LOGGER_HPP