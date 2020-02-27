// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "utils/Timer.hpp"

void tnt::Timer::start() noexcept
{
    if (isPaused)
        isPaused = false;
}
void tnt::Timer::stop() noexcept
{
    if (!isPaused)
        isPaused = true;
}

void tnt::Timer::reset() noexcept
{
    begin = std::chrono::steady_clock::now();
}

bool tnt::Timer::paused() const noexcept
{
    return isPaused;
}

long long tnt::Timer::deltaTime() const noexcept
{
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - begin).count();
}