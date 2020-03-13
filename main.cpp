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
