#ifdef TNT_USE_LUA

#ifndef TNT_LUA_MANAGER_HPP
#define TNT_LUA_MANAGER_HPP

#include <sol/sol.hpp>

// TODO: register AND test all engine types and functions.
// TODO: Add support for .tmx maps and text on Lua's AssetManager when available.
// TODO: remove the need of passing a state on each argument.

// TODO(maybe):
// keep a sol::state on the engine's context ??

namespace tnt::lua
{
    void loadVector(sol::state &lua_);       // done
    void loadRectangle(sol::state &lua_);    // done
    void loadTimer(sol::state &lua_);        // needs tests
    void loadAssetManager(sol::state &lua_); // needs tests
    void loadObject(sol::state &lua_);       // needs tests
    void loadMatrix(sol::state &lua_);       // TODO
    void loadInput(sol::state &lua_);        // TODO
    void loadVFS(sol::state &lua_);          // TODO
    void loadWidgets(sol::state &lua_);      // TODO
    void loadImGui(sol::state &lua_);        // TODO
    void loadUtils(sol::state &lua_);        // last_write_time, etc
    void loadCameras(sol::state &lua_);      // TODO
    // void loadRigidBody(sol::state& lua_);
    // void loadSprite(sol::state& lua_);

} // namespace tnt::lua

#endif //!TNT_LUA_MANAGER_HPP
#endif //!TNT_USE_LUA