// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <nlohmann/json.hpp>

#include "doo_ecs/Scripts.hpp"
#include "fileIO/VirtualFS.hpp"

#include "tolua/LuaLoader.hpp"
#include "tolua/LuaDooEcs.hpp"
#include "tolua/LuaUtils.hpp"

namespace tnt::doo
{
    void scripts_sys::add_object(object const &id, std::string_view filename,
                                 tnt::lua::lib const &libs)
    {
        [[unlikely]] if (id > states.capacity())
            states.reserve(10);

        active.insert(id, id);
        states.emplace(states.cbegin() + id);

        states[id].open_libraries(sol::lib::base);
        states[id]["id"] = id + 1;

        lua::load(states[id], libs);
        lua::loadDooEcs(states[id]);

        states[id].safe_script_file(vfs::absolute(filename));
    }

    void scripts_sys::Init(object const &id)
    {
        if (active.contains(id))
        {
            sol::safe_function init = states[id]["init"];
            if (auto const &res = init(); !res.valid())
            {
                sol::error err = res;
                logger::error(err.what());
            }
        }
    }

    void scripts_sys::Update(tnt::doo::object const &id, float time_)
    {
        if (active.contains(id))
        {
            sol::safe_function fn = states[id]["update"];
            if (auto const &res = fn(time_); !res.valid())
            {
                sol::error err = res;
                logger::error(err.what());
            }
        }
    }

    void scripts_sys::from_json(object const &id, nlohmann::json const &j)
    {
        if (j.contains("script"))
        {
            // if (j["script"].contains("libs"))
            // {
            // nlohmann::json const &jlib{j["script"]["libs"]};
            // lua::lib arr[]{j["script"]["libs"]};
            // lua::lib libs{};
            // for (int i{0}; i < std::size(arr); ++i)
            //     libs |= arr[i];
            //     add_object(id, j["script"]["file"], lua::lib::core | lua::lib::math | lua::lib::utils);
            // }
            // else
            add_object(id, j["script"]["file"]);
        }
    }

    void scripts_sys::remove(object const &id) noexcept
    {
        active.erase(id);
        states.erase(states.cbegin() + id);
    }

    void scripts_sys::clear() noexcept
    {
        active.clear();
        states.clear();
    }
} // namespace tnt::doo