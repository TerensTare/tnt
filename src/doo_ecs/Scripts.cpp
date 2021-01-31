// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <nlohmann/json.hpp>

#include "doo_ecs/Scripts.hpp"
#include "fileIO/VirtualFS.hpp"

#include "tolua/LuaLoader.hpp"
#include "tolua/LuaDooEcs.hpp"
#include "tolua/LuaUtils.hpp"

#include "utils/Benchmark.hpp"

namespace tnt::doo
{
    void scripts_sys::add_object(object const &id, std::string_view filename)
    {
        PROFILE_FUNCTION();

        [[unlikely]] if (id > states.capacity())
            states.reserve(10);

        active.insert(id, id);
        states.emplace(states.cbegin() + id);

        auto& s{states[id]};

        s.open_libraries(sol::lib::package, sol::lib::base);
        s["id"] = id + 1;
        s["file"] = vfs::absolute(filename);

        lua::registerLoader(s, true);
        lua::loadDooEcs(s);

        s.safe_script_file(vfs::absolute(filename));
    }

    void scripts_sys::Init(object const &id)
    {
        if (active.contains(id))
        {
            PROFILE_FUNCTION();

            sol::safe_function init = states[id]["init"];
            if (auto const &res = init(); !res.valid())
            {
                sol::error err = res;
                logger::error(err.what());
            }
        }
    }

    void scripts_sys::reload(object const &id) noexcept
    {
        PROFILE_FUNCTION();

        std::string tmp{states[id]["file"].get<std::string>()};
        remove(id);
        add_object(id, tmp);
    }

    void scripts_sys::Update(tnt::doo::object const &id, float time_)
    {
        if (active.contains(id))
        {
            PROFILE_FUNCTION();

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
            PROFILE_FUNCTION();
            add_object(id, j["script"]["file"]);
        }
    }

    void scripts_sys::to_json(object const &id, nlohmann::json &j)
    {
        PROFILE_FUNCTION();
        j["script"]["file"] = states[id]["file"];
    }

    void scripts_sys::remove(object const &id) noexcept
    {
        PROFILE_FUNCTION();
        active.erase(id);
        states.erase(states.cbegin() + id);
    }

    void scripts_sys::clear() noexcept
    {
        PROFILE_FUNCTION();
        active.clear();
        states.clear();
    }
} // namespace tnt::doo