#ifndef TNT_BENCH_STREAM_HPP
#define TNT_BENCH_STREAM_HPP

#include <string>
#include <memory_resource>

#include <fmt/os.h>

#include "core/Config.hpp"
#include "mirror/Any.hpp"

// TODO:
// overload for void() lambda which writes to the file.

TNT_EXPORT template class TNT_API std::chrono::time_point<std::chrono::high_resolution_clock>;

namespace tnt
{
    inline constexpr struct TNT_API start_batch_tag final
    {
        explicit start_batch_tag() = default;
    } start_batch{};

    inline constexpr struct TNT_API end_batch_tag final
    {
        explicit end_batch_tag() = default;
    } end_batch{};

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
        using unit = std::chrono::time_point<std::chrono::high_resolution_clock>;

        bool m_Stopped{false};
        std::hash<std::thread::id> hash{};
        const char *m_Name;
        unit m_StartTimepoint;
    };

    struct TNT_API BenchEmptyTimer final
    {
        explicit constexpr BenchEmptyTimer(const char *) noexcept {}
        constexpr void Stop() noexcept {}
    };

    using BenchTimer = std::conditional_t<
        is_profiling, BenchValidTimer, BenchEmptyTimer>;

    class TNT_API bench_stream final
    {
    public:
        explicit bench_stream(std::string_view name_ = "benchmark.json") noexcept;

        const char *filename() const noexcept;
        void write_to(std::string_view file_);

        void bench(any const &fn);

    private:
        char mem[256];
        std::pmr::monotonic_buffer_resource buffer{mem, 256};
        std::pmr::string name;

        fmt::buffered_file file;
    };

    TNT_API bench_stream &operator<<(bench_stream const &b, start_batch_tag const &);
    TNT_API bench_stream &operator<<(bench_stream const &b, end_batch_tag const &);
    TNT_API bench_stream &operator<<(bench_stream &b, std::string_view file);
} // namespace tnt

#endif //!TNT_BENCH_STREAM_HPP