// // This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// // PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <fstream>

#include "core/Window.hpp"

#include "doo_ecs/Animations.hpp"
#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Sprites.hpp"

#include "imgui/ImGui.hpp"

#include "utils/Timer.hpp"
#include "utils/Logger.hpp"

using tnt::doo::animations;
using tnt::doo::objects;
using tnt::doo::physics;
using tnt::doo::sprites;

int main(int argc, char **argv)
{
    tnt::Window window{"Data Oriented ECS example", 800, 600};

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

    tnt_imgui_init(window);

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
        {
            // update
            physics.Update(obj, dt);
            animations.Update(obj, dt);

            // draw
            if (sprites.draw_queue.size() > obj &&
                sprites.draw_queue[obj] != -1)
                sprites.Draw(obj, window);
        }

        if (tnt::ImGui::Begin(window, "Hello", 300, 100))
        {
            if (tnt::ImGui::BeginSection(window, "Test"))
            {
                tnt::ImGui::colored_text(window, "Some text", 255, 140, 0, 0);
                tnt::ImGui::EndSection();
            }

            {
                static float value{125678.23f};
                tnt::ImGui::hslider_float(window, "float", 1000.f, 200000.f, &value);
            }

            tnt::ImGui::End();
        }

        window.Render();
    }

    tnt_imgui_close();

    return 0;
}