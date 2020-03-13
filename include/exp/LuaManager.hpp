#ifndef LUA_MANAGER_HPP
#define LUA_MANAGER_HPP

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
// void loadRigidBody(sol::state& lua);
} // namespace tnt::lua

#endif //!LUA_MANAGER_HPP