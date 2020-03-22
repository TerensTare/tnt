#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <atomic>
#include <fstream>

namespace tnt
{
// TODO: handle deltaTime when paused
// TODO(maybe):
// asynchronous timer ??
// make Timer header-only ??
class Timer
{
public:
    Timer();

    void start() noexcept;
    void reset() noexcept;
    void stop() noexcept;

    bool paused() const noexcept;

    template <typename Duration = std::chrono::milliseconds, std::enable_if_t<std::chrono::_Is_duration_v<Duration>, int> = 0>
    Duration &deltaTime() noexcept(noexcept(std::chrono::duration_cast<Duration>(std::chrono::steady_clock::now() - begin)))
    {
        std::atomic_thread_fence(std::memory_order_relaxed);
        auto ret{std::chrono::duration_cast<Duration>(std::chrono::steady_clock::now() - begin)};
        std::atomic_thread_fence(std::memory_order_relaxed);
        return ret;
    }

private:
    bool isPaused;
    std::chrono::steady_clock::time_point begin;
};

template <typename F, typename... Args>
long long measure(F const &func, Args &&... args)
{
    auto start{std::chrono::steady_clock::now()};
    func(std::forward<Args...>(args...));

    std::atomic_thread_fence(std::memory_order_relaxed);
    auto ret{std::chrono::duration_cast<Duration>(std::chrono::steady_clock::now() - begin)};
    std::atomic_thread_fence(std::memory_order_relaxed);

    return ret;
}
} // namespace tnt

#endif //!TIMER_HPP