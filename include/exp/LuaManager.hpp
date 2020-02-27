#ifndef LUA_MANAGER_HPP
#define LUA_MANAGER_HPP

#include <sol/sol.hpp>
#include "physics/Vector.hpp"

namespace tnt::lua
{
void loadVector(sol::state const &lua)
{
    lua.new_usertype<Vector>(sol::constructors<Vector(float, float)>{}, "new");
}
void loadTimer(sol::state &lua);
void loadMatrix(sol::state &lua);
void loadAssetManager(sol::state &lua);
void loadVFS(sol::state &lua);
} // namespace tnt::lua

#endif //!LUA_MANAGER_HPP