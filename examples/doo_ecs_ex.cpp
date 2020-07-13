#include <fstream>

#include "core/Window.hpp"
#include "doo_ecs/Objects.hpp"

using tnt::doo::objects;
using tnt::doo::physics;
using tnt::doo::sprites;

int main(int argc, char **argv)
{
    tnt::Window window{"Data Oriented ECS example", 800, 600};

    nlohmann::json j;
    {
        std::ifstream i{"objects.json"};
        i >> j;
    }

    for (nlohmann::json const &it : j)
    {
        objects.from_json(it);
        sprites.from_json(window, it);
        physics.from_json(it);
    }

    SDL_Event e;

    while (window.isOpened())
    {
        while (SDL_PollEvent(&e))
            window.handleEvents(e);

        window.Clear();
        for (tnt::doo::object const &obj : objects.active)
        {
            // update
            physics.Update(obj, 1);
            objects.Update(obj, 1);

            // draw
            if (tnt::doo::object const s{sprites.draw_queue.size()};
                s > obj)
                objects.Draw(obj, window);
        }
        window.Render();
    }
    
    return 0;
}