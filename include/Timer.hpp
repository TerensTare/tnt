#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <fstream>

namespace tnt
{
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

} // namespace tnt

#endif //!TIMER_HPP