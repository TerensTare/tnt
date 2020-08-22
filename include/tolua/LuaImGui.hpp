#ifndef TNT_EXPORT_IMGUI_TO_LUA_HPP
#define TNT_EXPORT_IMGUI_TO_LUA_HPP

#include <sol/forward.hpp>

namespace tnt::lua
{
    void loadImGui(sol::state_view lua_);
} // namespace tnt::lua

#endif //!TNT_EXPORT_IMGUI_TO_LUA_HPP