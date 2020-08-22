#ifndef TNT_EXPORT_CORE_TO_LUA_HPP
#define TNT_EXPORT_CORE_TO_LUA_HPP

#include <sol/forward.hpp>

namespace tnt::lua
{
    void loadWindow(sol::state_view lua_);
    void loadInput(sol::state_view lua_);
    // void loadAudio(sol::state_view lua_); // TODO
} // namespace tnt::lua

#endif //!TNT_EXPORT_CORE_TO_LUA_HPP