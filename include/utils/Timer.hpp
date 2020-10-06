#ifndef TNT_TIMER_HPP
#define TNT_TIMER_HPP

#include <chrono>

// TODO(maybe):
// asynchronous timer ??

namespace tnt
{
    class Timer
    {
        using fast_milli = std::chrono::duration<float, std::milli>;

    public:
        inline void start() noexcept(noexcept(
            std::chrono::duration_cast<fast_milli>(
                std::chrono::steady_clock::now() - pausedTime)))
        {
            [[likely]] if (isPaused)
            {
                isPaused = false;
                [[likely]] if (pausedTime != beginning)
                    deltaPaused += std::chrono::duration_cast<fast_milli>(
                        std::chrono::steady_clock::now() - pausedTime);
                beginning = std::chrono::steady_clock::now();
            }
        }

        inline void reset() noexcept
        {
            isPaused = false;
            beginning = std::chrono::steady_clock::now();
            pausedTime = std::chrono::steady_clock::now();
            deltaPaused = fast_milli{0};
        }

        inline void stop() noexcept
        {
            isPaused = true;
            pausedTime = std::chrono::steady_clock::now();
        }

        inline bool paused() const noexcept { return isPaused; }

        inline fast_milli deltaTime() noexcept(noexcept(
            std::chrono::duration_cast<fast_milli>(
                std::chrono::steady_clock::now() - beginning - deltaPaused)))
        {
            start();
            fast_milli const deltaT = std::chrono::duration_cast<fast_milli>(
                std::chrono::steady_clock::now() - beginning - deltaPaused);
            reset();
            return deltaT;
        }

        inline float deltaCount() noexcept(noexcept(
            std::chrono::duration_cast<fast_milli>(
                std::chrono::steady_clock::now() - beginning - deltaPaused)))
        {
            start();
            fast_milli const &deltaT{
                std::chrono::duration_cast<fast_milli>(
                    std::chrono::steady_clock::now() - beginning - deltaPaused)};
            reset();
            return deltaT.count();
        }

    private:
        bool isPaused{false};
        std::chrono::steady_clock::time_point beginning{std::chrono::steady_clock::now()};
        std::chrono::steady_clock::time_point pausedTime{std::chrono::steady_clock::now()};
        fast_milli deltaPaused{0};
    };
} // namespace tnt

#endif //! TNT_TIMER_HPP