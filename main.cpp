// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <fstream>

#include "core/Window.hpp"
#include "core/Input.hpp"

#include "doo_ecs/Animations.hpp"
#include "doo_ecs/Cameras.hpp"
#include "doo_ecs/Objects.hpp"
#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Sprites.hpp"

#include "fileIO/VirtualFS.hpp"

#include "utils/Logger.hpp"
#include "utils/Timer.hpp"

using tnt::doo::animations;
using tnt::doo::cameras;
using tnt::doo::objects;
using tnt::doo::physics;
using tnt::doo::sprites;

int main(int argc, char **argv)
{
    tnt::Window window{"The TnT Engine", 800, 600};

    tnt::vfs::mount("assets", "res");
    tnt::vfs::mount("scripts", "lua");

    {
        nlohmann::json j;

        for (std::ifstream{"objects.json"} >> j;
             nlohmann::json const &it : j["objects"])
        {
            tnt::doo::object const &obj{objects.from_json(it)};

            physics.from_json(obj, it);
            sprites.from_json(obj, window, it);
            animations.from_json(obj, it);
        }

        for (nlohmann::json const &it : j["cameras"])
            cameras.from_json(it);
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

        float const &change{dt * .2f};

        if (tnt::input::keyDown(SDL_SCANCODE_LEFT))
            objects.pos[0].x -= change;
        else if (tnt::input::keyDown(SDL_SCANCODE_RIGHT))
            objects.pos[0].x += change;
        else if (tnt::input::keyDown(SDL_SCANCODE_UP))
            objects.pos[0].y -= change;
        else if (tnt::input::keyDown(SDL_SCANCODE_DOWN))
            objects.pos[0].y += change;
        else if (tnt::input::keyDown(SDL_SCANCODE_S))
            cameras.shake(0);

        cameras.follow(0, 0, dt);
        objects.angle[0] += (dt * .01f);

        window.Clear();
        for (tnt::doo::object const &obj : objects.active)
            if (obj != tnt::doo::null)
            {
                // update
                physics.Update(obj, dt);
                animations.Update(obj, dt);

                // draw
                sprites.Draw(obj, window, 0);
            }
        window.Render();
    }

    return 0;
}