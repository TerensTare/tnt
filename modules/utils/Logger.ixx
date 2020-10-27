module;

#include <concepts>
#include <fmt/core.h>
#include <fmt/color.h>

export module TnT.Logger;

// TODO: unfinished
// TODO: temporary use libfmt, but after MSVC will support C++20's <format>, use that instead.
// TODO: support writing to files.
// TODO: support writing logs to the developer console built in the engine.
// TODO: make sure that tnt::logger::debug writes sth only if DEBUG || _DEBUG is defined.
// TODO: correctly quit the application when tnt::logger::error is called and you are in debug mode.
// TODO: user defined literals for any logging type.
// && ex. "Initialized subsystem {}, {}..."_info.params(someParams,...).

// TODO(maybe):
// make this an extension ??
// write to html/xml files for better output.

// colors:
// trace -> dark slate blue
// debug -> white
// info -> white
// notice -> cyan
// warning -> yellow
// error -> red
// critical -> crimson

namespace detail
{
    inline constexpr fmt::text_style crimson{fmt::fg(fmt::color::crimson)};
    inline constexpr fmt::text_style cyan{fmt::fg(fmt::color::cyan)};
    inline constexpr fmt::text_style green{fmt::fg(fmt::color::dark_sea_green)};
    inline constexpr fmt::text_style red{fmt::fg(fmt::color::red)};
    inline constexpr fmt::text_style slate_blue{fmt::fg(fmt::color::slate_blue)};
    inline constexpr fmt::text_style yellow{fmt::fg(fmt::color::yellow)};
} // namespace detail

// NOTE: Priorities are in ascending order.
export namespace tnt::logger
{
    template <typename T>
    concept printable = requires(T t) { fmt::print("{}", t); };

    template <printable... Args>
    inline void trace(std::string_view format, Args &&... args)
    {
        if constexpr (sizeof...(Args) > 0)
            fmt::print(detail::slate_blue, "{} TRACE: {}\n", __TIME__,
                       fmt::format(format, std::forward<Args>(args)...));
        else
            fmt::print(detail::slate_blue, "{} TRACE: {}\n", __TIME__, format);
    }

    template <printable... Args>
    inline void debug(std::string_view format, Args &&... args)
    {
        if constexpr (sizeof...(Args) > 0)
            fmt::print("{} DEBUG: {}\n", __TIME__,
                       fmt::format(format, std::forward<Args>(args)...));
        else
            fmt::print("{} DEBUG: {}\n", __TIME__, format);
    }

    template <printable... Args>
    inline void info(std::string_view format, Args &&... args)
    {
        if constexpr (sizeof...(Args) > 0)
            fmt::print("{} INFO: {}\n", __TIME__,
                       fmt::format(format, std::forward<Args>(args)...));
        else
            fmt::print("{} INFO: {}\n", __TIME__, format);
    }

    template <printable... Args>
    inline void notice(std::string_view format, Args &&... args)
    {
        if constexpr (sizeof...(Args) > 0)
            fmt::print(detail::cyan, "{} NOTICE: {}\n", __TIME__,
                       fmt::format(format, std::forward<Args>(args)...));
        else
            fmt::print(detail::cyan, "{} NOTICE: {}\n", __TIME__, format);
    }

    template <printable... Args>
    inline void warning(std::string_view format, Args &&... args)
    {
        if constexpr (sizeof...(Args) > 0)
            fmt::print(detail::yellow,
                       "{} WARNING: {}\n", __TIME__,
                       fmt::format(format, std::forward<Args>(args)...));
        else
            fmt::print(detail::yellow, "{} WARNING: {}\n", __TIME__, format);
    }

    template <printable... Args>
    [[noreturn]] inline void error(std::string_view format, Args &&... args)
    {
        if constexpr (sizeof...(Args) > 0)
            fmt::print(detail::red, "{} ERROR: {}\n", __TIME__,
                       fmt::format(format, std::forward<Args>(args)...));
        else
            fmt::print(detail::red, "{} ERROR: {}\n", __TIME__, format);

        std::quick_exit(-1);
    }

    template <printable... Args>
    inline void critical(std::string_view format, Args &&... args)
    {
        if constexpr (sizeof...(Args) > 0)
            fmt::print(detail::crimson, "{} CRITICAL: {}\n", __TIME__,
                       fmt::format(format, std::forward<Args>(args)...));
        else
            fmt::print(detail::crimson, "{} CRITICAL: {}\n", __TIME__, format);
    }
} // namespace tnt::logger