// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <sol/sol.hpp>

#include "ecs/Scriptable.hpp"
#include "core/Context.hpp"

tnt::Script::Script(std::string_view filename) noexcept : file{filename} {}

void tnt::Script::Update(float time_) noexcept
{
    sol::state_view state{context.luaState()};
    state.do_file(file);
    state["update"](time_);
}