#ifndef TNT_EXPORT_DOO_ECS_TO_LUA_HPP
#define TNT_EXPORT_DOO_ECS_TO_LUA_HPP

#include <sol/forward.hpp>

namespace tnt::lua
{
    void loadDooEcs(sol::state_view lua_);
} // namespace tnt::lua

#endif //!TNT_EXPORT_DOO_ECS_TO_LUA_HPP