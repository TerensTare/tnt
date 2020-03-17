// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "core/Window.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    using tnt::Window;

    bool quit{false};
    SDL_Event e;

    Window *window{
        new Window{"Test",
                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                   800, 600,
                   SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE}};

    while (!quit)
    {
        SDL_WaitEvent(&e);
        switch (e.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        }

        window->Clear();
        window->Render();
    }

    delete window;
    return 0;
}
