#include <sol/sol.hpp>
#include "core/Context.hpp"

tnt::Context::Context() noexcept : lua_{} {}

lua_State *tnt::Context::luaState() const noexcept
{
    return lua_->operator lua_State *();
}