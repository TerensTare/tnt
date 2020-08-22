// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <algorithm>

#include "tolua/LuaLoader.hpp"

#include "tolua/LuaCore.hpp"
#include "tolua/LuaDooEcs.hpp"
#include "tolua/LuaImGui.hpp"
#include "tolua/LuaMath.hpp"

#include "core/Window.hpp"
#include "core/Input.hpp"
#include "fileIO/AudioPlayer.hpp"

auto luaLoad = [](char const *l) -> void {
    using tnt::lua_ctx;
    using namespace tnt::lua;

    if (l == "core")
    {
        loadWindow(lua_ctx);
        loadInput(lua_ctx);
        // loadAudio(lua_ctx);
    }
    else if (l == "math")
    {
        loadVector(lua_ctx);
        loadRect(lua_ctx);
    }
    else if (l == "ecs")
        loadDooEcs(lua_ctx);
    else if (l == "imgui")
        loadImGui(lua_ctx);
};

[[maybe_unused]] int ret = []() -> int {
    tnt::lua_ctx["load"] = &luaLoad;
    return 0;
}();

void tnt::lua::load(std::span<tnt::lua::lib> libs)
{
    for (lib const &l : libs)
        if (l == lib::core)
        {
            loadWindow(lua_ctx);
            loadInput(lua_ctx);
            // loadAudio(lua_ctx);
        }
        else if (l == lib::math)
        {
            loadVector(lua_ctx);
            loadRect(lua_ctx);
        }
        else if (l == lib::doo_ecs)
            loadDooEcs(lua_ctx);
        else if (l == lib::imgui)
            loadImGui(lua_ctx);
}