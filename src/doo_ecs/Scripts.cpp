// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "doo_ecs/Scripts.hpp"
#include "tolua/LuaLoader.hpp"

namespace tnt::doo
{
    void scripts_sys::add_object(std::string_view filename)
    {
        [[unlikely]] if (updates.size() == updates.capacity())
        {
            script_queue.reserve(10);
            updates.reserve(10);
        }

        script_queue.emplace_back(updates.size());
        lua_ctx.script_file(filename.data());

        updates.emplace_back(lua_ctx["update"]);
    }

    void scripts_sys::add_invalid()
    {
        [[unlikely]] if (updates.size() == updates.capacity())
        {
            script_queue.reserve(10);
            updates.reserve(10);
        }

        script_queue.emplace_back(null);
        updates.emplace_back(lua_ctx["nocollide_functionName"].get_or_create<sol::protected_function>());
    }

    void scripts_sys::Update(tnt::doo::object const &id, float time_)
    {
        updates[id](time_);
    }

    void scripts_sys::from_json(nlohmann::json const &j)
    {
        if (json_has(j, "script"))
            add_object(j["script"]);
        else
            add_invalid();
    }
} // namespace tnt::doo