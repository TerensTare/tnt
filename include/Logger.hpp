#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <future>

namespace tnt
{

// TODO: unfinished
// TODO: temporary use libfmt, but after MSVC will support C++20's <format>, use that instead.
class Logger
{
public:
    static Logger &This();

    template <typename... Args>
    void info(std::string format, Args &&... args);

    template <typename... Args>
    void warning(std::string format, Args &&... args);

    template <typename... Args>
    void error(std::string format, Args &&... args);

    template <typename... Args>
    void debug(std::string format, Args &&... args);

    template <typename... Args>
    void notice(std::string format, Args &&... args);

    template <typename... Args>
    void trace(std::string format, Args &&... args); // lowest

    template <typename... Args>
    void critical(std::string format, Args &&... args); // highest

private:
    Logger();
    ~Logger() noexcept;
};
} // namespace tnt

#endif //!LOGGER_HPP