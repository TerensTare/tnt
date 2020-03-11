#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <atomic>
#include <fstream>

namespace tnt
{
// TODO(maybe): asynchronous timer ??
// TODO: add function to benchmark another function.
// TODO: update the value of begin (maybe) in deltaTime ??
// TODO(maybe): make Timer header-only ??
class Timer
{
public:
    Timer();

    void start() noexcept;
    void reset() noexcept;
    void stop() noexcept;

    bool paused() const noexcept;

    template <typename Duration, std::enable_if_t<std::chrono::_Is_duration_v<Duration>, int> = 0>
    Duration &deltaTime() const noexcept
    {
        std::atomic_thread_fence(std::memory_order_relaxed);
        auto ret{std::chrono::duration_cast<Duration>(std::chrono::steady_clock::now() - begin)};
        std::atomic_thread_fence(std::memory_order_relaxed);
        return ret;
    }

private:
    bool isPaused{false};
    std::chrono::steady_clock::time_point begin;
};

template <typename F, typename... Args>
long long measure(F const &func, Args &&... args)
{
    auto start{std::chrono::steady_clock::now()};
    func(std::forward<Args...>(args...));
    auto end{std::chrono::steady_clock::now()};

    return std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
}
} // namespace tnt

#endif //!TIMER_HPP