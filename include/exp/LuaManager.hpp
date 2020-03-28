#ifdef TNT_USE_LUA

#ifndef TNT_LUA_MANAGER_HPP
#define TNT_LUA_MANAGER_HPP

#include <sol/sol.hpp>

// TODO:
// register AND test all engine types and functions.
namespace tnt::lua
{
void loadVector(sol::state &lua);
void loadTimer(sol::state &lua);
void loadMatrix(sol::state &lua);
void loadAssetManager(sol::state &lua);
void loadVFS(sol::state &lua);
void loadWidgets(sol::state &lua);
void loadUtils(sol::state &lua); // last_write_time, etc
// void loadRigidBody(sol::state& lua);

} // namespace tnt::lua

#endif //!TNT_LUA_MANAGER_HPP
#endif //!TNT_USE_LUA