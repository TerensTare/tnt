#ifndef TNT_LUA_SANDBOX_HPP
#define TNT_LUA_SANDBOX_HPP

#include <sol/sol.hpp>

#include "core/Config.hpp"

// TODO: add bindings for logger, widgets, Expected<T> etc.
namespace tnt
{
class [[deprecated(
    "LuaSandbox is not recommended (as it is part of the engine,NOT the SDK). Use LuaManager instead.")]] LuaSandbox
{
public:
    static LuaSandbox &This();

    sol::state *operator->() { return &lua; }

private:
    LuaSandbox();

    void loadVector();
    void loadMatrix();
    // void loadVFS();
    void loadCamera();
    void loadTimer();
    void loadObject();
    void loadEasings();

    static sol::state lua;
};
} // namespace tnt

#endif //! TNT_LUA_SANDBOX_HPP