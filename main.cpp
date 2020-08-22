// // This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// // PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// #include "core/Input.hpp"
// #include "core/Window.hpp"

// #include "ecs/Sprite.hpp"

// #include "imgui/ImGui.hpp"

// #include "fileIO/Snipper.hpp"

// #include "utils/Timer.hpp"
// #include "utils/LuaManager.hpp"

// namespace fs = std::filesystem;

// // TODO:
// // define window params on script, or config.lua
// // add a sprite type for lua to access it on scripts
// // add option --install, same as --pack but target gets exported to SDL_GetPrefPath()
// // add support for building project files
// // use the new data oriented ecs
// // export objects.json, if it exists

// class Player final
//     : public tnt::Sprite
// {
// public:
//     inline explicit Player(tnt::Window const *win) noexcept
//         : tnt::Sprite{win, "assets/player.png",
//                       tnt::Rectangle{3.f, 0.f, 10.f, 16.f}} {}

//     inline void Update(float time_) noexcept override { return; }
// };

// inline void loadPlayer(sol::state_view lua_)
// {
//     lua_.new_enum("coords",
//                   "local", Player::LOCAL,
//                   "global", Player::GLOBAL);

//     lua_.new_usertype<Player>(
//         "player", sol::constructors<Player(tnt::Window const *) noexcept>{},

//         "angle", sol::property([](Player const &obj) { return obj.getAngle(Player::LOCAL); }, &Player::setAngle),
//         "gAngle", [](Player const &obj) { return obj.getAngle(Player::GLOBAL); },
//         "rotate", &Player::Rotate,

//         "scale", sol::property([](Player const &obj) { return obj.getScale(Player::LOCAL); }, &Player::setScale),
//         "gScale", [](Player const &obj) { return obj.getScale(Player::GLOBAL); },
//         "rescale", &Player::Scale,

//         "pos", sol::property([](Player const &obj) { return obj.getPosition(Player::LOCAL); }, &Player::setPosition),
//         "gPos", [](Player const &obj) { return obj.getPosition(Player::GLOBAL); },
//         "transform", &Player::Transform,

//         "parent", sol::property(&Player::getParent, &Player::setParent),
//         "active", sol::property(&Player::isActive, &Player::setActive),

//         "w", sol::property(&Player::getWidth),
//         "h", sol::property(&Player::getHeight),

//         "update", &Player::Update,
//         "draw", &Player::Draw);
// }

// auto pack = [](std::string_view filename) -> void {
//     std::string_view name{filename.substr(0, filename.size() - 4)};
//     fs::create_directory(name); // create a directory with the name of the .lua file, but remove the extension
//     const fs::path dir{name};

//     tnt::logger::info("Packaging project {}.\n", name);

//     fs::copy_file(filename, dir / filename);
//     fs::copy("assets", dir / "assets");

//     for (fs::directory_iterator end,
//          it{fs::current_path()};
//          it != end; ++it)
//     {
//         const fs::path file{it->path()};
//         const fs::path ext{file.extension()};
//         if (ext == ".dll" || ext == ".so" || ext == ".dylib")
//             fs::copy_file(file, dir / file.filename());
//     }

//     tnt::logger::info("Succesfully packaged project {}.\n", name);
// };

// auto run = [](std::string_view filename) -> void {
//     tnt::Window *window{new tnt::Window{
//         "The TnT Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//         1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE}};

//     long long dt{0};
//     SDL_DisplayMode display;
//     SDL_GetDisplayMode(0, 0, &display);

//     tnt::Timer timer;
//     tnt::Snipper snipper;

//     tnt::lua::lib libs_impl[]{
//         tnt::lua::lib::MATH, tnt::lua::lib::WINDOW,
//         tnt::lua::lib::IMGUI, tnt::lua::lib::SPRITE_COMP,
//         tnt::lua::lib::PHYS_COMP, tnt::lua::lib::SCENE};
//     std::span libs{libs_impl};

//     sol::state lua_;
//     SDL_Event e;

//     lua_.open_libraries(sol::lib::base);
//     tnt::lua::load(lua_, libs);
//     loadPlayer(lua_);

//     lua_["screen_w"] = display.w;
//     lua_["screen_h"] = display.h;

//     auto tnt{lua_["tnt"].get_or_create<sol::table>()};

//     lua_.script_file(filename.data());

//     sol::protected_function update{tnt["update"]};
//     sol::protected_function draw{tnt["draw"]};
//     sol::protected_function do_imgui{tnt["do_imgui"].get_or_create<sol::function>()}; // do_imgui is optional

//     tnt_imgui_init(window);
//     tnt["init"](window);

//     while (window->isOpened())
//     {
//         dt = timer.deltaTime().count();
//         timer.reset();

//         while (SDL_PollEvent(&e))
//         {
//             window->handleEvents(e);
//             if (e.type == SDL_WINDOWEVENT &&
//                 e.window.event == SDL_WINDOWEVENT_RESIZED)
//                 SDL_GetDisplayMode(0, 0, &display);

//             tnt::ImGui::update_context();
//         }

//         if (snipper.isModified(filename))
//         {
//             lua_.script_file(filename.data());
//             tnt = lua_["tnt"];

//             update = tnt["update"];
//             draw = tnt["draw"];
//             do_imgui = tnt["do_imgui"];
//         }

//         if (dt != 0)
//             update(dt);

//         window->Clear();

//         draw(window);
//         do_imgui(window);

//         window->Render();

//         SDL_Delay(1);
//     }

//     tnt_imgui_close();

//     delete window;
// };

// int main(int argc, char **argv)
// {
//     if (argc != 3)
//     {
//         fmt::print("Incorrect number of arguments passed!!\n"
//                    "\t--run <filename>.lua\t\tRun <filename>.lua\n"
//                    "\t--pack <filename>.lua\t\tPackage <filename>.lua into <filename> directory.");
//         return -1;
//     }

//     const std::string_view cmd{argv[1]};

//     if (cmd == "-r" || cmd == "--run")
//         run(argv[2]);
//     else if (cmd == "-p" || cmd == "--pack")
//         pack(argv[2]);
//     else
//         [[unlikely]] tnt::logger::info(
//             "Detected invalid flag: {}!!\n"
//             "Supported flags are -r/--run and -p/--pack",
//             argv[1]);

//     return 0;
// }

#include <fstream>

#include "core/Window.hpp"
#include "core/Input.hpp"

#include "doo_ecs/Animations.hpp"
#include "doo_ecs/Physics.hpp"

#include "utils/Timer.hpp"

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
        window.Render();
    }

    return 0;
}