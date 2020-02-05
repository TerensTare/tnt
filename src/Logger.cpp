#include <fmt/core.h>
#include "Logger.hpp"

tnt::Logger &tnt::Logger::This()
{
    static Logger inst;
    return inst;
}

template <class... Args>
void tnt::Logger::info(std::string format, Args &&... args)
{
    (fmt::format(format, args), ...);
}

template <typename... Args>
void tnt::Logger::warning(std::string format, Args &&... args) {}

template <typename... Args>
void tnt::Logger::error(std::string format, Args &&... args) {}

template <typename... Args>
void tnt::Logger::debug(std::string format, Args &&... args) {}

template <typename... Args>
void tnt::Logger::notice(std::string format, Args &&... args) {}

template <typename... Args>
void tnt::Logger::trace(std::string format, Args &&... args) {}

template <typename... Args>
void tnt::Logger::critical(std::string format, Args &&... args) {}