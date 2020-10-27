#ifndef TNT_EXPORT_IMGUI_TO_LUA_HPP
#define TNT_EXPORT_IMGUI_TO_LUA_HPP

#include <sol/forward.hpp>
#include "core/Config.hpp"

namespace tnt::lua
{
    TNT_API void loadImGui(sol::state_view lua_);
} // namespace tnt::lua

#endif //!TNT_EXPORT_IMGUI_TO_LUA_HPP