// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "doo_ecs/Scripts.hpp"
#include "fileIO/VirtualFS.hpp"
#include "tolua/LuaLoader.hpp"
#include "utils/Logger.hpp"

namespace
{
    std::vector<sol::state> states; /// < The lua handles of the objects.
}

namespace tnt::doo
{
    void scripts_sys::add_object(object const &id, std::string_view filename)
    {
        [[unlikely]] if (id > states.capacity())
        {
            script_queue.reserve(10);
            states.reserve(10);
        }

        script_queue.emplace(script_queue.cbegin() + id, id);
        states.emplace(states.cbegin() + id);

        states[id].open_libraries(sol::lib::base);
        states[id]["id"] = id + 1;
        lua::lib libs[]{lua::lib::core, lua::lib::math,
                        lua::lib::doo_ecs};
        lua::load(states[id], libs);

        if (auto const &res = states[id].safe_script_file(
                vfs::absolute(filename));
            !res.valid())
        {
            sol::error err = res;
            logger::error(err.what());
        }
    }

    void scripts_sys::Init(object const &id)
    {
        if (!has_object(script_queue, id))
            return;

        sol::safe_function init = states[id]["init"];

        if (auto const &res = init(); !res.valid())
        {
            sol::error err = res;
            logger::error(err.what());
        }
    }

    void scripts_sys::Update(tnt::doo::object const &id, float time_)
    {
        if (!has_object(script_queue, id))
            return;

        sol::safe_function fn = states[id]["update"];
        if (auto const &res = fn(time_); !res.valid())
        {
            sol::error err = res;
            logger::error(err.what());
        }
    }

    void scripts_sys::from_json(object const &id, nlohmann::json const &j)
    {
        if (json_has(j, "script"))
            add_object(id, j["script"]);
    }
} // namespace tnt::doo