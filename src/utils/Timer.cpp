// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <atomic>
#include "utils/Timer.hpp"

tnt::Timer::Timer()
    : isPaused{false}, deltaPaused{0},
      beginning{std::chrono::steady_clock::now()} {}

void tnt::Timer::start() noexcept
{
    if (isPaused)
    {
        deltaPaused = deltaPaused +
                      std::chrono::duration_cast<std::chrono::milliseconds>(
                          std::chrono::steady_clock::now() - pausedTime);
        isPaused = false;
    }
}
void tnt::Timer::stop() noexcept
{
    if (!isPaused)
    {
        pausedTime = std::chrono::steady_clock::now();
        isPaused = true;
    }
}

void tnt::Timer::reset() noexcept
{
    // std::atomic_thread_fence(std::memory_order_relaxed);
    beginning = std::chrono::steady_clock::now();
    // std::atomic_thread_fence(std::memory_order_relaxed);
}

bool tnt::Timer::paused() const noexcept { return isPaused; }
