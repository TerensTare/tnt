// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "core/Input.hpp"
#include "core/Window.hpp"

#include "ecs/Sprite.hpp"

#include "imgui/ImGui.hpp"

#include "utils/Timer.hpp"
#include "utils/LuaManager.hpp"

// TODO: "dissolve" this code into classes, like Game/Scene/Space, etc.

// TODO:
// define window params on script, or config.lua
// add a sprite type for lua to access it on scripts
// hot code reloading

class Player final
    : public tnt::Sprite
{
public:
    explicit Player(tnt::Window const *win) noexcept
        : tnt::Sprite{win, "assets/player.png",
                      tnt::Rectangle{3.f, 0.f, 10.f, 16.f}} {}

    inline void Update(long long time_) noexcept override { return; }
};

inline void loadPlayer(sol::state_view lua_)
{
    lua_.new_enum("coords",
                  "local", Player::LOCAL,
                  "global", Player::GLOBAL);

    lua_.new_usertype<Player>(
        "player", sol::constructors<Player(tnt::Window const *) noexcept>{},

        "angle", sol::property([](Player const &obj) { return obj.getAngle(Player::LOCAL); }, &Player::setAngle),
        "gAngle", [](Player const &obj) { return obj.getAngle(Player::GLOBAL); },
        "rotate", &Player::Rotate,

        "scale", sol::property([](Player const &obj) { return obj.getScale(Player::LOCAL); }, &Player::setScale),
        "gScale", [](Player const &obj) { return obj.getScale(Player::GLOBAL); },
        "rescale", &Player::Scale,

        "pos", sol::property([](Player const &obj) { return obj.getPosition(Player::LOCAL); }, &Player::setPosition),
        "gPos", [](Player const &obj) { return obj.getPosition(Player::GLOBAL); },
        "transform", &Player::Transform,

        "parent", sol::property(&Player::getParent, &Player::setParent),
        "active", sol::property(&Player::isActive, &Player::setActive),

        "w", sol::property(&Player::getWidth),
        "h", sol::property(&Player::getHeight),

        "update", &Player::Update,
        "draw", &Player::Draw);
}

int main(int argc, char **argv)
{
    tnt::Window *window{new tnt::Window{
        "The TnT Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE}};

    long long dt{0};
    SDL_DisplayMode display;
    SDL_GetDisplayMode(0, 0, &display);

    tnt::Timer timer;

    tnt::lua::lib libs_impl[]{
        tnt::lua::lib::MATH, tnt::lua::lib::WINDOW,
        tnt::lua::lib::IMGUI, tnt::lua::lib::SPRITE_COMP,
        tnt::lua::lib::PHYS_COMP, tnt::lua::lib::SCENE};
    std::span libs{libs_impl};

    sol::state lua_;
    SDL_Event e;

    lua_.open_libraries(sol::lib::base, sol::lib::package);
    tnt::lua::load(lua_, libs);
    loadPlayer(lua_);

    lua_["screen_w"] = display.w;
    lua_["screen_h"] = display.h;

    lua_.script_file("./game.lua");

    auto update{lua_["update"]};
    auto draw{lua_["draw"]};
    auto do_imgui{lua_["do_imgui"]};

    lua_["init"](window);

    while (window->isOpened())
    {
        dt = timer.deltaTime().count();
        timer.reset();

        while (SDL_PollEvent(&e))
        {
            window->handleEvents(e);
            if (e.type == SDL_WINDOWEVENT &&
                e.window.event == SDL_WINDOWEVENT_RESIZED)
                SDL_GetDisplayMode(0, 0, &display);

            tnt::ImGui::update_context();
        }

        if (dt != 0)
            update(dt);

        window->Clear();

        draw(window);
        do_imgui(window);

        window->Render();

        SDL_Delay(1);
    }

    tnt_imgui_close();
    tnt::input::close();

    delete window;

    return 0;
}