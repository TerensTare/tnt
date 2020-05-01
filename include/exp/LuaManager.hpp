#ifdef TNT_USE_LUA

#ifndef TNT_LUA_MANAGER_HPP
#define TNT_LUA_MANAGER_HPP

#include <sol/sol.hpp>

// TODO: register AND test all engine types and functions.
// TODO: Add support for .tmx maps and text on Lua's AssetManager when available.

namespace tnt::lua
{
void loadVector(sol::state &lua);       // done
void loadRectangle(sol::state &lua);    // done
void loadTimer(sol::state &lua);        // needs tests
void loadAssetManager(sol::state &lua); // needs tests
void loadMatrix(sol::state &lua);       // TODO
void loadInput(sol::state &lua);        // TODO
void loadVFS(sol::state &lua);          // TODO
void loadWidgets(sol::state &lua);      // TODO
void loadImGui(sol::state &lua);        // TODO
void loadUtils(sol::state &lua);        // last_write_time, etc
// void loadRigidBody(sol::state& lua);

} // namespace tnt::lua

#endif //! TNT_LUA_MANAGER_HPP
#endif //! TNT_USE_LUA