#ifndef TNT_EXPORT_CORE_TO_LUA_HPP
#define TNT_EXPORT_CORE_TO_LUA_HPP

#include <sol/forward.hpp>
#include "core/Config.hpp"

namespace tnt::lua
{
    TNT_API void loadWindow(sol::state_view lua_);
    TNT_API void loadInput(sol::state_view lua_);
    inline void loadAudio(sol::state_view lua_){}; // TODO
} // namespace tnt::lua

#endif //!TNT_EXPORT_CORE_TO_LUA_HPP