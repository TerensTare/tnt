#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <chrono>
#include <thread>

#include "core/Config.hpp"

// TODO: more noexcept stuff here

// TODO(maybe): make this thread-safe ??
// TODO(maybe): make this an extension ??

// code taken from
// https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e

namespace tnt
{
    namespace bench
    {
        TNT_API void BeginSession(std::string_view name,
                                  std::string_view filepath);
        TNT_API void EndSession();
    } // namespace bench

#ifdef TNT_PROFILING
    inline constexpr bool is_profiling{true};
#else
    inline constexpr bool is_profiling{false};
#endif

    class TNT_API BenchValidTimer final
    {
    public:
        explicit BenchValidTimer(const char *name) noexcept;
        ~BenchValidTimer();

        void Stop();

    private:
        std::hash<std::thread::id> hash;
        bool m_Stopped;
        const char *m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    };

    struct BenchEmptyTimer final
    {
        explicit constexpr BenchEmptyTimer(const char *) noexcept {}
        constexpr void Stop() noexcept {}
    };

    using BenchTimer = std::conditional_t<
        is_profiling, BenchValidTimer, BenchEmptyTimer>;
} // namespace tnt

#define PROFILE_SCOPE(name) \
    tnt::BenchTimer timer##__LINE__ { name }
#if defined(_MSC_VER) && !defined(__clang__)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_FUNCTION() PROFILE_SCOPE(__PRETTY_FUNCTION__)
#endif

#endif //! BENCHMARK_HPP