// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>
#include <nfd.h>

#if defined(_MSC_VER) and not defined(__clang__)
#pragma warning(push)
#pragma warning(disable : 4275)
#endif

#include "core/Window.hpp"
#include "core/Input.hpp"

#include "doo_ecs/Animations.hpp"
#include "doo_ecs/Bones.hpp"
#include "doo_ecs/Cameras.hpp"
#include "doo_ecs/Objects.hpp"
#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Sprites.hpp"
#include "doo_ecs/Scripts.hpp"

#include "imgui/ImGui.hpp"
#include "fileIO/VirtualFS.hpp"

#include "utils/Timer.hpp"
#include "utils/Benchmark.hpp"

#if defined(_MSC_VER) and not defined(__clang__)
#pragma warning(pop)
#endif

using tnt::doo::animations;
using tnt::doo::bones;
using tnt::doo::cameras;
using tnt::doo::objects;
using tnt::doo::physics;
using tnt::doo::scripts;
using tnt::doo::sprites;

// TODO:
// save to specific project.
// customizable keybindings.
// zoom in/out.
// export to executable.
// extensions.
// multi-window support.
// (maybe)
// access systems in a Data Oriented way
// ie. access all objects of a system then continue with the others

auto load_proj = [](tnt::Window const &window, nlohmann::json const &j, nfdchar_t *base) {
    tnt::safe_ensure(j.contains("name"), "Project file doesn't have \"name\" field!!");
    tnt::safe_ensure(j.contains("width"), "Project file doesn't have \"width\" field!!");
    tnt::safe_ensure(j.contains("height"), "Project file doesn't have \"height\" field!!");

    if (objects.active.size() > 0)
    {
        physics.clear();
        bones.clear();
        animations.clear();
        sprites.clear();
        scripts.clear();
        objects.clear();
    }

    for (nlohmann::json const &it : j["objects"])
    {
        std::filesystem::path p{base};
        p = p.parent_path();
        std::filesystem::current_path(p);
        p /= it.get<std::string_view>();
        tnt::logger::debug("Loading object from {}.", p.string());

        nlohmann::json object;
        std::ifstream{p.c_str()} >> object;

        tnt::doo::object const &obj{objects.from_json(object)};

        physics.from_json(obj, object);
        sprites.from_json(obj, window, object);
        animations.from_json(obj, object);
        scripts.from_json(obj, object);
        bones.from_json(obj, object);
    }

    if (j.contains("cameras"))
        for (nlohmann::json const &it : j["cameras"])
        {
            tnt::logger::debug("Loading camera from {}.", it.get<std::string_view>());

            nlohmann::json c;
            std::ifstream{tnt::vfs::absolute(it)} >> c;

            cameras.from_json(c);
        }
};

auto save_proj = []([[maybe_unused]] nlohmann::json &proj) {};

auto update_from_input = [](tnt::doo::object const &active, float const dt) noexcept -> void {
    float const &dst{dt * .05f};                     // 10 pixel/sec
    float const &rot{dt * .02f};                     // 20 degree/sec
    tnt::Vector const &zoom{dt * .001f, dt * .001f}; // 1 pixel/sec

    if (tnt::input::keyDown(SDL_SCANCODE_LEFT))
        objects.pos[active].x -= dst;
    else if (tnt::input::keyDown(SDL_SCANCODE_RIGHT))
        objects.pos[active].x += dst;
    else if (tnt::input::keyDown(SDL_SCANCODE_UP))
        objects.pos[active].y -= dst;
    else if (tnt::input::keyDown(SDL_SCANCODE_DOWN))
        objects.pos[active].y += dst;
    else if (tnt::input::keyDown(SDL_SCANCODE_Q))
        objects.angle[active] -= rot;
    else if (tnt::input::keyDown(SDL_SCANCODE_W))
        objects.angle[active] += rot;
    else if (tnt::input::keyDown(SDL_SCANCODE_S))
        objects.scale[active] += zoom;
    else if (tnt::input::keyDown(SDL_SCANCODE_A))
        objects.scale[active] -= zoom;
};

auto doo_loop = [](tnt::Window const &window, tnt::doo::object &active, float dt, bool const game) noexcept -> void {
    auto const &[x, y]{tnt::input::mousePosition()};
    tnt::Vector const &pos{(float)x, (float)y};

    for (tnt::doo::object const &obj : objects.active)
    {
        if (auto const &area{sprites.draw_area(obj)};
            area.Contains(pos) && tnt::input::mouseButtonDown(0))
            active = obj;

        if (game)
        {
            scripts.Update(obj, dt);
            physics.Update(obj, dt);
            animations.Update(obj, dt);
        }

        // draw
        sprites.Draw(obj, window);
    }
};

auto draw_imgui = [](tnt::Window const &window, tnt::doo::object &active, float dt,
                     bool &project, bool &game, bool &help) noexcept -> void {
    static constexpr const char *game_text[]{"Play", "Pause"};
    static constexpr const char *help_text[]{"Show Help", "Hide Help"};

    if (tnt::ImGui::Begin(window, "Components", 500, 300))
    {
        if (project)
            tnt::ImGui::text(window, fmt::format("{} ms, {} fps", dt, 1000 / dt));

        tnt::ImGui::newline();
        if (tnt::ImGui::button(window, "Load"))
        {
            if (nfdchar_t * name{nullptr}; NFD_OpenDialog("TnT", nullptr, &name) == NFD_OKAY)
            {
                tnt::logger::info("Loading project \"{}\"...", name);

                nlohmann::json p;
                std::ifstream{name} >> p;

                load_proj(window, p, name);

                SDL_Window *win = (SDL_Window *)const_cast<tnt::Window &>(window);

                SDL_SetWindowTitle(win, p["name"].get<std::string_view>().data());
                SDL_SetWindowSize(win, p["width"].get<int>(), p["height"].get<int>());

                project = true;

                tnt::logger::info("Successfully loaded project \"{}\" from {}",
                                  p["name"], name);

                free(name);
            }
        }

        if (project && tnt::ImGui::button(window, game_text[game]))
            game = !game;

        if (project && tnt::ImGui::button(window, help_text[help]))
            help = !help;

        if (project && !game && (active != tnt::doo::null))
        {
            objects.draw_imgui(active, window);
            physics.draw_imgui(active, window);
        }

        tnt::ImGui::End();
    }

    if (project && !game && help && tnt::ImGui::Begin(window, "Help", 0, 400))
    {
        tnt::ImGui::text(window, "Controls:");
        tnt::ImGui::text(window, "Up, Left, Down, Right: Move the current active object.");
        tnt::ImGui::text(window, "S, A: Scale Up/Down");
        tnt::ImGui::text(window, "Q, W: Rotate Left/Right");

        tnt::ImGui::End();
    }
};

int main(int argc, char **argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    tnt::bench::BeginSession("Editor", "benchmark.json");

    tnt::Window window{"The TnT Engine", 800, 600};
    window.setIcon("assets/TnT.ico");

    bool game{false};
    bool help{false};
    bool project{false};

    tnt::doo::object active{tnt::doo::null};

    tnt::Timer timer;
    SDL_Event e;

    tnt::vfs::mount("assets", "res");

    tnt_imgui_init(window);

    for (float dt{timer.deltaCount()}; window.isOpened(); dt = timer.deltaCount())
    {
        while (SDL_PollEvent(&e))
            window.handleEvents(e);
        tnt::input::Update();

        if (project && !game && (active != tnt::doo::null))
            update_from_input(active, dt);

        window.Clear();

        doo_loop(window, active, game, dt);
        draw_imgui(window, active, dt, project, game, help);

        window.Render();
    }

    return 0;
}