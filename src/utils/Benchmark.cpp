// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <algorithm>
#include <fstream>
#include <string>
#include <thread>

#include "utils/Benchmark.hpp"

inline static int profileCount{0};
inline static std::string current{""};
inline static std::string file{""};
inline static std::ofstream out;

inline static struct safe_close final
{
    inline ~safe_close()
    {
        if (!current.empty())
            tnt::bench::EndSession();
    }
} _;

inline static auto WriteHeader = [] {
    out << "{\"otherData\": {},\"traceEvents\":[";
};

inline static auto WriteFooter = [] {
    out << "]}";
};

struct ProfileResult final
{
    std::string Name;
    long long Start, End;
    std::size_t ThreadID;
};

inline static void WriteProfile(ProfileResult const &result)
{
    if (profileCount++ > 0)
        out << ",";

    // TODO: optimize these 2 lines
    std::string name = result.Name;
    std::replace(name.begin(), name.end(), '"', '\'');
    // TODO: optimize this. (maybe) by using fmt::format ??
    out << "{";
    out << "\"cat\":\"function\",";
    out << "\"dur\":" << (result.End - result.Start) << ',';
    out << "\"name\":\"" << name << "\",";
    out << "\"ph\":\"X\",";
    out << "\"pid\":0,";
    out << "\"tid\":" << result.ThreadID << ",";
    out << "\"ts\":" << result.Start;
    out << "}";
}

namespace tnt
{
    namespace bench
    {
        void BeginSession(std::string_view name, std::string_view filepath)
        {
            if constexpr (is_profiling)
            {
                if (!current.empty())
                    EndSession();

                current = name;

                if (file != filepath)
                {
                    out.open(filepath.data());
                    WriteHeader();
                    file = filepath;
                }
            }
        }

        void EndSession()
        {
            if constexpr (is_profiling)
            {
                WriteFooter();
                profileCount = 0;
                current.clear();
                file.clear();
                out.close();
            }
        }
    } // namespace bench

    BenchValidTimer::BenchValidTimer(const char *name) noexcept
        : m_Name{name}, m_Stopped{false},
          m_StartTimepoint{std::chrono::high_resolution_clock::now()} {}

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
        std::size_t threadID =
            std::hash<std::thread::id>{}(std::this_thread::get_id());
        WriteProfile({m_Name, start, end, threadID});

        m_Stopped = true;
    }
} // namespace tnt