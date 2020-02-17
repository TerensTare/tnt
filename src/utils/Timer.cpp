// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Timer.hpp"

void tnt::Timer::start() noexcept { isPaused = false; }
void tnt::Timer::stop() noexcept { isPaused = true; }

long long tnt::Timer::deltaTime() const noexcept
{
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - begin).count();
}
