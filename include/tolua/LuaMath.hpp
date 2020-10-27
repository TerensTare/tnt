#ifndef TNT_EXPORT_MATH_TO_LUA_HPP
#define TNT_EXPORT_MATH_TO_LUA_HPP

#include <sol/forward.hpp>
#include "core/Config.hpp"

namespace tnt::lua
{
    TNT_API void loadVector(sol::state_view lua_);
    TNT_API void loadRect(sol::state_view lua_);
} // namespace tnt::lua

#endif //!TNT_EXPORT_MATH_TO_LUA_HPP