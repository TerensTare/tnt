// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <sol/sol.hpp>

#include "core/Graphics.hpp"
#include "core/Window.hpp"
#include "core/Input.hpp"

#include "tolua/LuaCore.hpp"

void tnt::lua::loadWindow(sol::state_view lua_)
{
    lua_.new_usertype<Window>(
        "window", sol::constructors<Window(std::string_view, int, int, int, int, Uint32), Window(std::string_view, int, int)>{},

        "clear", &Window::Clear, "render", &Window::Render);
    //,
    // "set_color", sol::resolve<void(Uint8, Uint8, Uint8, Uint8)>(&Window::setClearColor));
    // "opened", &Window::isOpened, "handleEvents", &Window::handleEvents);
}

void tnt::lua::loadInput(sol::state_view lua_)
{
    auto input{lua_["input"].get_or_create<sol::table>()};

    // keyboard stuff
    input["key_down"] = &input::keyDown;
    input["key_pressed"] = &input::keyPressed;
    input["key_released"] = &input::keyReleased;
    input["last_key"] = &input::lastKeyPressed;

    // mouse stuff
    input["button_down"] = &input::mouseButtonDown;
    input["button_pressed"] = &input::mouseButtonPressed;
    input["button_released"] = &input::mouseButtonReleased;
    input["last_button"] = &input::lastMouseButton;

    input["mouse_pos"] = &input::mousePosition;

    input["update"] = &input::updateCurrent;
    input["update_last"] = &input::updatePrevious;
}

void tnt::lua::loadGraphics(sol::state_view lua_)
{
    auto gfx{lua_["gfx"].get_or_create<sol::table>()};

    gfx["line"] = &drawLine;
    gfx["hline"] = &drawHorizontalLine;
    gfx["vline"] = &drawVerticalLine;
    gfx["lines"] = &drawLines;
    gfx["circle_line"] = &drawCircle;
    gfx["circle"] = &fillCircle;
}