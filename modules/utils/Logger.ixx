module;
#include <fmt/core.h>
export module TnT.Utils:Logger;

// TODO: unfinished
// TODO: make colored output, and maybe write to html/xml files for better output.
// TODO: temporary use libfmt, but after MSVC will support C++20's <format>, use that instead.
// TODO: support writing to files, maybe by using fmt::vprint.
// TODO: support writing logs to the developer console built in the engine.
// TODO: make sure that tnt::logger::debug writes sth only if DEBUG || _DEBUG is defined.
// TODO: correctly quit the application when tnt::logger::error is called and you are in debug mode.
// TODO: user defined literals for any logging type.
// && ex. "Initialized subsystem x"_info.

// Priorities are in ascending order:
export namespace tnt::logger
{
    template <typename T>
    inline void trace(std::string_view format, T data)
    {
        fmt::print("{} TRACE: {}\n", __TIME__,
                   fmt::format(format, std::forward<T>(data)));
    }

    template <typename... Args>
    inline void trace(std::string_view format, Args &&... args)
    {
        fmt::print("{} TRACE: {}\n", __TIME__,
                   fmt::format(format, std::forward<Args>(args)...));
    }

    template <>
    inline void trace(std::string_view format)
    {
        fmt::print("{} TRACE: {}\n", __TIME__, format);
    }

    template <typename T>
    inline void debug(std::string_view format, T data)
    {
        fmt::print("{} DEBUG: {}\n", __TIME__,
                   fmt::format(format, std::forward<T>(data)));
    }

    template <typename... Args>
    inline void debug(std::string_view format, Args &&... args)
    {
        fmt::print("{} DEBUG: {}\n", __TIME__,
                   fmt::format(format, std::forward<Args>(args)...));
    }

    template <>
    inline void debug(std::string_view format)
    {
        fmt::print("{} DEBUG: {}\n", __TIME__, format);
    }

    template <typename T>
    inline void info(std::string_view format, T data)
    {
        fmt::print("{} INFO: {}\n", __TIME__,
                   fmt::format(format, std::forward<T>(data)));
    }

    template <class... Args>
    inline void info(std::string_view format, Args &&... args)
    {
        fmt::print("{} INFO: {}\n", __TIME__,
                   fmt::format(format, std::forward<Args>(args)...));
    }

    template <>
    inline void info(std::string_view format)
    {
        fmt::print("{} INFO: {}\n", __TIME__, format);
    }

    template <typename T>
    inline void notice(std::string_view format, T data)
    {
        fmt::print("{} NOTICE: {}\n", __TIME__,
                   fmt::format(format, std::forward<T>(data)));
    }

    template <typename... Args>
    inline void notice(std::string_view format, Args &&... args)
    {
        fmt::print("{} NOTICE: {}\n", __TIME__,
                   fmt::format(format, std::forward<Args>(args)...));
    }

    template <>
    inline void notice(std::string_view format)
    {
        fmt::print("{} NOTICE: {}\n", __TIME__, format);
    }

    template <typename T>
    inline void warning(std::string_view format, T data)
    {
        fmt::print("{} WARNING: {}\n", __TIME__,
                   fmt::format(format, std::forward<T>(data)));
    }

    template <typename... Args>
    inline void warning(std::string_view format, Args &&... args)
    {
        fmt::print("{} WARNING: {}\n", __TIME__,
                   fmt::format(format, std::forward<Args>(args)...));
    }

    template <>
    inline void warning(std::string_view format)
    {
        fmt::print("{} WARNING: {}\n", __TIME__, format);
    }

    template <typename T>
    inline void error(std::string_view format, T data)
    {
        fmt::print("{} ERROR: {}\n", __TIME__,
                   fmt::format(format, std::forward<T>(data)));
        std::quick_exit(-1);
    }

    template <typename... Args>
    inline void error(std::string_view format, Args &&... args)
    {
        fmt::print("{} ERROR: {}\n", __TIME__,
                   fmt::format(format, std::forward<Args>(args)...));
        std::quick_exit(-1);
    }

    template <>
    inline void error(std::string_view format)
    {
        fmt::print("{} ERROR: {}\n", __TIME__, format);
        std::quick_exit(-1);
    }

    template <typename T>
    inline void critical(std::string_view format, T data)
    {
        fmt::print("{} CRITICAL: {}\n", __TIME__,
                   fmt::format(format, std::forward<T>(data)));
    }

    template <typename... Args>
    inline void critical(std::string_view format, Args &&... args)
    {
        fmt::print("{} CRITICAL: {}\n", __TIME__,
                   fmt::format(format, std::forward<Args>(args)...));
    }

    template <>
    inline void critical(std::string_view format)
    {
        fmt::print("{} CRITICAL: {}\n", __TIME__, format);
    }
} // namespace tnt::logger
