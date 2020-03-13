#include "core/Graphics.hpp"
#include "gui/Window.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    using tnt::Graphics, tnt::Window;
    bool quit{false};
    SDL_Event e;

    Window *win{
        new Window{"Test",
                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                   800, 600, SDL_WINDOW_SHOWN}};

    while (!quit)
    {
        SDL_WaitEvent(&e);
        switch (e.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        }
    }

    delete win;
    return 0;
}

// #include "utils/Benchmark.hpp"
// #include "utils/Logger.hpp"

// int main()
// {
//     tnt::Instrumentor::Get().BeginSession("new logging");
//     {
//         PROFILE_SCOPE("logger");
//         tnt::logger::info("hello {} and hello {}!", "world", "tnt");
//     }
//     tnt::Instrumentor::Get().EndSession();
// }

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
