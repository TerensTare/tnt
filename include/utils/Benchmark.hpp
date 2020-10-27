#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <chrono>
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
                          std::string_view filepath = "benchmark.json");
        TNT_API void EndSession();
    } // namespace bench

#ifdef TNT_PROFILING
    inline static constexpr bool is_profiling{true};
#else
    inline static constexpr bool is_profiling{false};
#endif

    class TNT_API BenchValidTimer final
    {
    public:
        explicit BenchValidTimer(const char *name) noexcept;
        ~BenchValidTimer();

        void Stop();

    private:
        const char *m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock>
            m_StartTimepoint;
        bool m_Stopped;
    };

    struct TNT_API BenchEmptyTimer final
    {
        inline explicit BenchEmptyTimer(const char *) noexcept {}
        inline void Stop() noexcept {}
    };

    using BenchTimer = std::conditional_t<
        is_profiling, BenchValidTimer, BenchEmptyTimer>;
} // namespace tnt

// clang-format off
#define PROFILE_SCOPE(name) \
    tnt::BenchTimer timer## __LINE__ { name }
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)

#endif //! BENCHMARK_HPP