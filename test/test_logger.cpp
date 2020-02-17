#include "utils/Logger.hpp"
#include "utils/Benchmark.hpp"

int main()
{
    tnt::Instrumentor::Get().BeginSession("testing logging methods");
    {
        PROFILE_SCOPE("old logger");
        tnt::logger::info("hello {}, {} and even {}", 1, 2, 3);
    }
    tnt::Instrumentor::Get().EndSession();
}