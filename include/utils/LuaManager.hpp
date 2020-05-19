#ifndef TNT_LUA_MANAGER_HPP
#define TNT_LUA_MANAGER_HPP

#include <sol/sol.hpp>
#include "core/Context.hpp"

// TODO: register AND test all engine types and functions.
// TODO: Add support for .tmx maps and text on Lua's AssetManager when available.
// TODO: remove the need of passing a state on each argument.

// TODO(maybe):
// keep a sol::state on the engine's context ??

namespace tnt::lua
{
    void loadVector(sol::state_view lua_ = context.luaState());    // done
    void loadRectangle(sol::state_view lua_ = context.luaState()); // done
    void loadTimer(sol::state_view lua_ = context.luaState());     // needs tests
    void loadObject(sol::state_view lua_ = context.luaState());    // needs tests
    void loadInput(sol::state_view lua_ = context.luaState());     // needs tests
    void loadWindow(sol::state_view lua_ = context.luaState());    // needs test TODO(maybe): add more mebers functions
    void loadCameras(sol::state_view lua_ = context.luaState());   // TODO
    void loadImGui(sol::state_view lua_ = context.luaState());     // TODO
    void loadVFS(sol::state_view lua_ = context.luaState());       // TODO
    void loadWidgets(sol::state_view lua_ = context.luaState());   // TODO
    void loadUtils(sol::state_view lua_ = context.luaState());     // last_write_time, etc
    // void loadAssetManager(sol::state const&lua_); // needs tests
    // void loadAudioPlayer(sol::state const&lua_); // needs tests
    // void loadComponents(sol::state const&lua_);  // needs tests TODO: add more components
    // void loadSprite(sol::state const&lua_);      // TODO
    // void loadMatrix(sol::state const&lua_);       // TODO
    // void loadRigidBody(sol::state& lua_);

    /// @brief Load all binding functions that are NOT marked as TODO.
    void loadAll(sol::state_view lua_ = context.luaState());
} // namespace tnt::lua

#endif //!TNT_LUA_MANAGER_HPP