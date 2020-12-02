// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "tolua/LuaLoader.hpp"

#include "tolua/LuaCore.hpp"
#include "tolua/LuaImGui.hpp"
#include "tolua/LuaMath.hpp"
#include "tolua/LuaUtils.hpp"

#include "core/Window.hpp"
#include "core/Input.hpp"
#include "fileIO/AudioPlayer.hpp"

// auto luaLoad = [](char const *l) -> void {
//     using tnt::lua_ctx;
//     using namespace tnt::lua;

//     if (l == "core")
//     {
//         loadWindow(lua_ctx);
//         loadInput(lua_ctx);
//         // loadAudio(lua_ctx);
//     }
//     else if (l == "math")
//     {
//         loadVector(lua_ctx);
//         loadRect(lua_ctx);
//     }
//     else if (l == "ecs")
//         loadDooEcs(lua_ctx);
//     else if (l == "imgui")
//         loadImGui(lua_ctx);
// };

void tnt::lua::load(sol::state_view lua_, tnt::lua::lib const &libs)
{
    if (has_flag(libs, lib::window))
        loadWindow(lua_);
    if (has_flag(libs, lib::input))
        loadInput(lua_);
    if (has_flag(libs, lib::audio))
        loadAudio(lua_);
    if (has_flag(libs, lib::vector))
        loadVector(lua_);
    if (has_flag(libs, lib::rect))
        loadRect(lua_);
    if (has_flag(libs, lib::imgui))
        loadImGui(lua_);
    if (has_flag(libs, lib::utils))
        loadSparseSet(lua_);
}