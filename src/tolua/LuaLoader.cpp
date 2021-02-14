// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "tolua/LuaLoader.hpp"

#include "tolua/LuaCore.hpp"
#include "tolua/LuaImGui.hpp"
#include "tolua/LuaMath.hpp"
#include "tolua/LuaUtils.hpp"

#include "utils/Logger.hpp"

inline static int lua_custom_require(lua_State *L)
{
    using namespace tnt::lua;
    std::string const pack{sol::stack::get<std::string>(L, 1)};

    if (pack == "vector")
        loadVector(L);
    else if (pack == "rect")
        loadRect(L);
    else if (pack == "window")
        loadWindow(L);
    else if (pack == "input")
        loadInput(L);
    else if (pack == "audio")
        loadAudio(L);
    else if (pack == "graphics")
        loadGraphics(L);
    else if (pack == "imgui")
        loadImGui(L);
    else if (pack == "utils")
        loadSparseSet(L);
    else
        tnt::logger::error("[lua] Trying to load unknown module {}!!", pack);

    std::string const script{fmt::format("return {}", pack)};
    luaL_loadbuffer(L, script.data(), script.size(), pack.c_str());

    return 1;
}

void tnt::lua::load(sol::state_view lua_, tnt::lua::lib const &libs)
{
    if (has_flag(libs, lib::window))
        loadWindow(lua_);
    if (has_flag(libs, lib::input))
        loadInput(lua_);
    if (has_flag(libs, lib::audio))
        loadAudio(lua_);
    if (has_flag(libs, lib::graphics))
        loadGraphics(lua_);
    if (has_flag(libs, lib::vector))
        loadVector(lua_);
    if (has_flag(libs, lib::rect))
        loadRect(lua_);
    if (has_flag(libs, lib::imgui))
        loadImGui(lua_);
    if (has_flag(libs, lib::utils))
        loadSparseSet(lua_);
}

void tnt::lua::registerLoader(lua_State *L, bool clear_old) noexcept
{
    sol::state_view lua_{L};
    lua_.add_package_loader(lua_custom_require, clear_old);
}