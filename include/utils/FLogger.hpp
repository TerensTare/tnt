#ifndef TNT_FILE_LOGGER_HPP
#define TNT_FILE_LOGGER_HPP

#include <fmt/os.h>

#include "utils/Logger.hpp"

namespace tnt::logger
{
    template <detail::printable... Args>
    inline void trace(fmt::ostream &file, std::string_view format, Args &&...args)
    {
        if constexpr (sizeof...(Args) > 0)
            file.print("{} [TRACE] {}\n", __TIME__,
                       fmt::format(format, std::forward<Args>(args)...));
        else
            file.print("{} [TRACE] {}\n", __TIME__, format);
    }

    template <detail::printable... Args>
    inline void debug(fmt::ostream &file, std::string_view format, Args &&...args)
    {
        if constexpr (sizeof...(Args) > 0)
            file.print("{} [DEBUG] {}\n", __TIME__,
                       fmt::format(format, std::forward<Args>(args)...));
        else
            file.print("{} [DEBUG] {}\n", __TIME__, format);
    }

    template <detail::printable... Args>
    inline void info(fmt::ostream &file, std::string_view format, Args &&...args)
    {
        if constexpr (sizeof...(Args) > 0)
            file.print("{} [INFO] {}\n", __TIME__,
                       fmt::format(format, std::forward<Args>(args)...));
        else
            file.print("{} [INFO] {}\n", __TIME__, format);
    }

    template <detail::printable... Args>
    inline void notice(fmt::ostream &file, std::string_view format, Args &&...args)
    {
        if constexpr (sizeof...(Args) > 0)
            file.print("{} [NOTICE] {}\n", __TIME__,
                       fmt::format(format, std::forward<Args>(args)...));
        else
            file.print("{} [NOTICE] {}\n", __TIME__, format);
    }

    template <detail::printable... Args>
    inline void warning(fmt::ostream &file, std::string_view format, Args &&...args)
    {
        if constexpr (sizeof...(Args) > 0)
            file.print("{} [WARNING] {}\n", __TIME__,
                       fmt::format(format, std::forward<Args>(args)...));
        else
            file.print("{} [WARNING] {}\n", __TIME__, format);
    }

    template <detail::printable... Args>
    [[noreturn]] inline void error(fmt::ostream &file, std::string_view format, Args &&...args)
    {
        if constexpr (sizeof...(Args) > 0)
            file.print("{} [ERROR] {}\n", __TIME__,
                       fmt::format(format, std::forward<Args>(args)...));
        else
            file.print("{} [ERROR] {}\n", __TIME__, format);

        std::quick_exit(-1);
    }

    template <detail::printable... Args>
    inline void critical(fmt::ostream &file, std::string_view format, Args &&...args)
    {
        if constexpr (sizeof...(Args) > 0)
            file.print("{} [CRITICAL] {}\n", __TIME__,
                       fmt::format(format, std::forward<Args>(args)...));
        else
            file.print("{} [CRITICAL] {}\n", __TIME__, format);
    }
} // namespace tnt::logger

#endif //!TNT_FILE_LOGGER_HPP