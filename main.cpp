// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <fstream>
#include <nlohmann/json.hpp>

#include "core/Window.hpp"
#include "core/Input.hpp"

#include "doo_ecs/Animations.hpp"
#include "doo_ecs/Cameras.hpp"
#include "doo_ecs/Objects.hpp"
#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Sprites.hpp"

#include "imgui/ImGui.hpp"

#include "fileIO/VirtualFS.hpp"

#include "utils/Logger.hpp"
#include "utils/Timer.hpp"

using tnt::doo::animations;
using tnt::doo::cameras;
using tnt::doo::objects;
using tnt::doo::physics;
using tnt::doo::sprites;

// TODO: load/save from specific project.

int main(int argc, char **argv)
{
    tnt::Window window{"The TnT Engine", 800, 600};

    tnt::vfs::mount("assets", "res");

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
    bool game{false};
    const char *button_text[]{"Play", "Pause"};
    tnt::doo::object active{0};

    tnt::Timer timer;
    SDL_Event e;

    tnt_imgui_init(window);
    sprites.target_cam(0);

    while (window.isOpened())
    {
        dt = timer.deltaCount();
        tnt::logger::info("{} ms, {} fps", dt, 1000 / dt);

        while (SDL_PollEvent(&e))
            window.handleEvents(e);

        auto const &[x, y]{tnt::input::mousePosition()};
        tnt::Vector const &pos{(float)x, (float)y};

        window.Clear();

        for (tnt::doo::object const &obj : objects.active)
        {
            if (auto const &area{sprites.draw_area(obj)};
                area.Contains(pos) && tnt::input::mouseButtonDown(0))
                active = obj;

            if (game)
            {
                physics.Update(obj, dt);
                animations.Update(obj, dt);
            }

            // draw
            sprites.Draw(obj, window);
        }

        if (tnt::ImGui::Begin(window, "Components", 400, 300))
        {
            tnt::ImGui::newline();
            if (tnt::ImGui::button(window, button_text[game]))
                game = !game;

            if (!game)
            {
                objects.draw_imgui(active, window);
                physics.draw_imgui(active, window);
            }

            tnt::ImGui::End();
        }

        window.Render();
    }

    return 0;
}