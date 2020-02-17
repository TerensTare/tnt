#include "utils/Benchmark.hpp"
#include "utils/Logger.hpp"

int main()
{
    tnt::Instrumentor::Get().BeginSession("new logging");
    {
        PROFILE_SCOPE("logger");
        tnt::logger::info("hello {} and hello {}!", "world", "tnt");
    }
    tnt::Instrumentor::Get().EndSession();
}

// #include "LuaSandbox.hpp"

// int main()
// {
//     auto lua{tnt::LuaSandbox::This()};
//     try
//     {
//         lua->do_string(R"(
//             obj = object:new(vector:new(10, 100), 0.9, vector:new(1, 1))
//             print(obj.angle)
//         )");
//     }
//     catch (...)
//     {
//     }
//     return 0;
// }
