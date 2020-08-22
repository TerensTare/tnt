#ifndef TNT_IMPORT_DATA_FROM_LUA_HPP
#define TNT_IMPORT_DATA_FROM_LUA_HPP

#include <span>
#include <sol/sol.hpp>

namespace tnt
{
    inline sol::state lua_ctx;
}

namespace tnt::lua
{
    enum class lib : uint32_t
    {
        math,
        core,
        imgui,
        doo_ecs
    };

    void load(std::span<tnt::lua::lib> libs);
} // namespace tnt::lua

#endif //!TNT_IMPORT_DATA_FROM_LUA_HPP