// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "utils/Timer.hpp"

tnt::Timer::Timer()
    : begin{std::chrono::steady_clock::now()},
      isPaused{false} {}

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
    std::atomic_thread_fence(std::memory_order_relaxed);
    begin = std::chrono::steady_clock::now();
    std::atomic_thread_fence(std::memory_order_relaxed);
}

bool tnt::Timer::paused() const noexcept { return isPaused; }