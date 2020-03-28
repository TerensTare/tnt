#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <atomic>

// TODO: handle deltaTime when paused
// TODO: reduce the number of std::atomic_thread_fence(std::memory_order_relaxed); calls

// TODO(maybe):
// asynchronous timer ??
// make Timer header-only ??

namespace tnt
{
class Timer
{
public:
    Timer();

    void start() noexcept;
    void reset() noexcept;
    void stop() noexcept;

    bool paused() const noexcept;

    template <typename Duration = std::chrono::milliseconds>
    Duration &deltaTime() noexcept(noexcept(std::chrono::duration_cast<Duration>(std::chrono::steady_clock::now() - beginning)))
    {
        std::atomic_thread_fence(std::memory_order_relaxed);
        auto ret{std::chrono::duration_cast<Duration>(std::chrono::steady_clock::now() - beginning)};
        // std::atomic_thread_fence(std::memory_order_relaxed);
        return ret;
    }

private:
    bool isPaused;
    std::chrono::steady_clock::time_point beginning;
};

} // namespace tnt

#endif //!TIMER_HPP