#ifndef EXP_LOGGER_HPP
#define EXP_LOGGER_HPP

#include <string>

namespace tnt::logger
{
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

} // namespace tnt::logger

#endif //!EXP_LOGGER_HPP