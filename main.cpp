#include "Benchmark.hpp"

int main(int, char **)
{
    using namespace tnt;
    Instrumentor::Get().BeginSession("testing strings");
    {
    }
    Instrumentor::Get().EndSession();

    return 0;
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
