#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fmt/core.h>

// TODO: unfinished
// TODO(partial): try calling fmt::print less.
// TODO: temporary use libfmt, but after MSVC will support C++20's <format>, use that instead.
// TODO: support writing to files, maybe by using fmt::vprint.
// TODO: support writing logs to the developer console built in the engine.
// TODO: make sure that tnt::logger::debug writes sth only if DEBUG || _DEBUG is defined.
// TODO: throw when called tnt::logger::error.
// TODO: correctly quit the application when tnt::logger::error is called.

// TODO(maybe): make this an extension.
// TODO(maybe): add macro TNT_TERMINATE_ON_ERROR.

// Priorities are in ascending order:
namespace tnt::logger
{
template <typename T>
void trace(std::string_view format, T data)
{
    fmt::print(__TIME__);
    fmt::print(" TRACE: {}\n", fmt::format(format, std::forward<T>(data)));
}

template <typename... Args>
void trace(std::string_view format, Args &&... args)
{
    fmt::print(__TIME__);
    fmt::print(" TRACE: {}\n", fmt::format(format, std::forward<Args>(args)...));
}

template <>
void trace(std::string_view format)
{
    fmt::print(__TIME__);
    fmt::print(" TRACE: ", format);
}

template <typename T>
void debug(std::string_view format, T data)
{
    fmt::print(__TIME__);
    fmt::print(" DEBUG: {}\n", fmt::format(format, std::forward<T>(data)));
}

template <typename... Args>
void debug(std::string_view format, Args &&... args)
{
    fmt::print(__TIME__);
    fmt::print(" DEBUG: {}\n", fmt::format(format, std::forward<Args>(args)...));
}

template <>
void debug(std::string_view format)
{
    fmt::print(__TIME__);
    fmt::print(" DEBUG: {}\n", format);
}

template <typename T>
void info(std::string_view format, T data)
{
    fmt::print(__TIME__);
    fmt::print(" INFO: {}\n", fmt::format(format, std::forward<T>(data)));
}

template <class... Args>
void info(std::string_view format, Args &&... args)
{
    fmt::print(__TIME__);
    fmt::print(" INFO: {}\n", fmt::format(format, std::forward<Args>(args)...));
}

template <>
void info(std::string_view format)
{
    fmt::print(__TIME__);
    fmt::print(" INFO: {}\n", format);
}

template <typename T>
void notice(std::string_view format, T data)
{
    fmt::print(__TIME__);
    fmt::print(" NOTICE: {}\n", fmt::format(format, std::forward<T>(data)));
}

template <typename... Args>
void notice(std::string_view format, Args &&... args)
{
    fmt::print(__TIME__);
    fmt::print(" NOTICE: {}\n", fmt::format(format, std::forward<Args>(args)...));
}

template <>
void notice(std::string_view format)
{
    fmt::print(__TIME__);
    fmt::print(" NOTICE: {}\n", format);
}

template <typename T>
void warning(std::string_view format, T data)
{
    fmt::print(__TIME__);
    fmt::print(" WARNING: {}\n", fmt::format(format, std::forward<T>(data)));
}

template <typename... Args>
void warning(std::string_view format, Args &&... args)
{
    fmt::print(__TIME__);
    fmt::print(" WARNING: {}\n", fmt::format(format, std::forward<Args>(args)...));
}

template <>
void warning(std::string_view format)
{
    fmt::print(__TIME__);
    fmt::print(" WARNING: {}\n", format);
}

template <typename T>
void error(std::string_view format, T data)
{
    fmt::print(__TIME__);
    fmt::print(" ERROR: {}\n", fmt::format(format, std::forward<T>(data)));
    std::quick_exit(-1);
}

template <typename... Args>
void error(std::string_view format, Args &&... args)
{
    fmt::print(__TIME__);
    fmt::print(" ERROR: {}\n", fmt::format(format, std::forward<Args>(args)...));
    std::quick_exit(-1);
}

template <>
void error(std::string_view format)
{
    fmt::print(__TIME__);
    fmt::print(" ERROR: {}\n", format);

    std::quick_exit(-1);
}

template <typename T>
void critical(std::string_view format, T data)
{
    fmt::print(__TIME__);
    fmt::print(" CRITICAL: {}\n", fmt::format(format, std::forward<T>(data)));
}

template <typename... Args>
void critical(std::string_view format, Args &&... args)
{
    fmt::print(__TIME__);
    fmt::print(" CRITICAL: {}\n", fmt::format(format, std::forward<Args>(args)...));
}

template <>
void critical(std::string_view format)
{
    fmt::print(__TIME__);
    fmt::print(" CRITICAL: {}\n", format);
}
} // namespace tnt::logger

#endif //!LOGGER_HPP