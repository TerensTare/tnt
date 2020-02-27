#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <fstream>

namespace tnt
{
// TODO(maybe): asynchronous timer?
// TODO: add function to benchmark another function.
class Timer
{
public:
    void start() noexcept;
    void reset() noexcept;
    void stop() noexcept;

    bool paused() const noexcept;
    long long deltaTime() const noexcept;

private:
    bool isPaused{false};
    // float scale{1.f}; // 1 means second
    std::chrono::steady_clock::time_point begin{std::chrono::steady_clock::now()};
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