module;
#include <atomic>
#include <chrono>
export module TnT.Utils : Timer;

// TODO: handle deltaTime when paused
// TODO: reduce the number of
// std::atomic_thread_fence(std::memory_order_relaxed); calls

// TODO(maybe):
// asynchronous timer ??
// make Timer header-only ??

export namespace tnt
{
    class Timer
    {
    public:
        Timer();

        void start() noexcept;
        void reset() noexcept;
        void stop() noexcept;

        bool paused() const noexcept;

        std::chrono::milliseconds deltaTime() noexcept(noexcept(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - beginning - deltaPaused)));

    private:
        bool isPaused;
        std::chrono::steady_clock::time_point beginning;
        std::chrono::steady_clock::time_point pausedTime;
        std::chrono::milliseconds deltaT;
        std::chrono::milliseconds deltaPaused;
    };
} // namespace tnt

#endif //! TNT_TIMER_HPP