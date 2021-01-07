// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "utils/Bench.hpp"

namespace tnt
{
    BenchValidTimer::BenchValidTimer(const char *name) noexcept
        : m_Name{name}, m_StartTimepoint{std::chrono::high_resolution_clock::now()} {}

    BenchValidTimer::~BenchValidTimer()
    {
        if (!m_Stopped)
            Stop();
    }

    void BenchValidTimer::Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start =
            std::chrono::time_point_cast<std::chrono::microseconds>(
                m_StartTimepoint)
                .time_since_epoch()
                .count();
        long long end =
            std::chrono::time_point_cast<std::chrono::microseconds>(
                endTimepoint)
                .time_since_epoch()
                .count();

        // (maybe) store a global hash object to avoid constructing every Stop() call ??
        std::size_t threadID = hash(std::this_thread::get_id());
        WriteProfile({m_Name, start, end, threadID});

        m_Stopped = true;
    }

    bench_stream::bench_stream(std::string_view name_)
        : name{name.data(), &buffer}, file{name_.data(), "a"} {}

    const char *bench_stream::filename() const noexcept { return name.data(); }

    void bench_stream::write_to(std::string_view file_)
    {
        name = file_.data();
        file = fmt::buffered_file{name.data(), "a"};
    }

    void bench_stream::bench(any const &fn)
    {
        auto fptr = any_cast<void (*)()>(fn);
        file.print(R"({{
            "cat" : "function",
            "dur" : "{}",
            "name" : "{}",
            "ph" : "X",
            "pid" : 0,
            "tid" : "{}",
            "ts" : "{}"
        }})");
    }

    bench_stream &operator<<(bench_stream &b, std::string_view file)
    {
        return (b.write_to(file), b);
    }
} // namespace tnt