// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <fstream>

#include "core/Window.hpp"
#include "core/Input.hpp"

#include "doo_ecs/Animations.hpp"
#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Sprites.hpp"

#include "utils/Logger.hpp"
#include "utils/Timer.hpp"

using tnt::doo::animations;
using tnt::doo::objects;
using tnt::doo::physics;
using tnt::doo::sprites;

int main(int argc, char **argv)
{
    tnt::Window window{"The TnT Engine", 800, 600};

    {
        nlohmann::json j;

        for (std::ifstream{"objects.json"} >> j;
             nlohmann::json const &it : j)
        {
            objects.from_json(it);
            physics.from_json(it);
            sprites.from_json(window, it);
            animations.from_json(it);
        }
    }

    float dt{0.f};

    tnt::Timer timer;
    SDL_Event e;

    while (window.isOpened())
    {
        dt = timer.deltaCount();
        tnt::logger::info("{} ms, {} fps", dt, 1000 / dt);

        while (SDL_PollEvent(&e))
            window.handleEvents(e);

        window.Clear();
        for (tnt::doo::object const &obj : objects.active)
            if (obj != -1)
            {
                // update
                physics.Update(obj, dt);
                animations.Update(obj, dt);

                // draw
                sprites.Draw(obj, window);
            }
        window.Render();
    }

    return 0;
}