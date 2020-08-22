#ifndef TNT_EXPORT_MATH_TO_LUA_HPP
#define TNT_EXPORT_MATH_TO_LUA_HPP

#include <sol/forward.hpp>

namespace tnt::lua
{
    void loadVector(sol::state_view lua_);
    void loadRect(sol::state_view lua_);
} // namespace tnt::lua

#endif //!TNT_EXPORT_MATH_TO_LUA_HPP