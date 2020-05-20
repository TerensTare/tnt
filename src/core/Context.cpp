#include "core/Context.hpp"
#include "utils/LuaManager.hpp"

tnt::Context::Context() noexcept : lua_{}
{
    lua_->open_libraries(sol::lib::base, sol::lib::package);
    lua::loadAll(*lua_);
}

lua_State *tnt::Context::luaState() const noexcept
{
    return lua_->operator lua_State *();
}