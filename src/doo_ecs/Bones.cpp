// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <nlohmann/json.hpp>

#include "doo_ecs/Bones.hpp"
#include "doo_ecs/Objects.hpp"

#include "json/JsonVector.hpp"

#include "utils/Benchmark.hpp"

namespace tnt::doo
{
    void bones_sys::add_object(object const &id, bone_comp const &data_)
    {
        PROFILE_FUNCTION();

        if (id == length.capacity())
        {
            length.reserve(10);
            joint.reserve(10);
        }

        active.push_back(id);
        length.emplace_back(data_.length);
        joint.emplace_back(data_.joint);
    }

    void bones_sys::from_json(object const &id, nlohmann::json const &j)
    {
        if (j.contains("bone"))
        {
            PROFILE_FUNCTION();
            add_object(id, {.joint{j["bone"]["joint"].get<tnt::Vector>()},
                            .length{j["bone"]["length"]}});
        }
    }

    void bones_sys::to_json(object const &id, nlohmann::json &j)
    {
        if (active.contains(id))
        {
            PROFILE_FUNCTION();

            nlohmann::json &bone{j["bone"]};
            bone["joint"] = joint[id];
            bone["length"] = length[id];
        }
    }

    void bones_sys::remove(object const &id) noexcept
    {
        PROFILE_FUNCTION();
        active.erase(id);
        length.erase(length.cbegin() + id);
        joint.erase(joint.cbegin() + id);
    }

    void bones_sys::clear() noexcept
    {
        PROFILE_FUNCTION();
        active.clear();
        length.clear();
        joint.clear();
    }

    Vector bones_sys::bottom(object const &id) const noexcept
    {
        PROFILE_FUNCTION();
        return {length[id] * cosf(objects.gAngle(id)), length[id] * sinf(objects.gAngle(id))};
    }

    // thx The Coding Train
    // https://www.youtube.com/watch?v=hbgDqyy8bIw
    void bones_sys::follow(object const &id, Vector const &target) noexcept
    {
        PROFILE_FUNCTION();

        Vector const &dir{target - joint[id] - objects.gPos(id)};
        objects.angle[id] += AngleOf(dir) - objects.gAngle(id);

        // the last bone can't move, only rotate
        if (objects.parent[id] != null)
        {
            objects.pos[id] = target - (dir.Normalized() * length[id]);
            joint[id] = target - dir.Normalized() * length[id] - objects.gPos(id);
        }

        for (object obj{id}, parent{objects.parent[obj]};
             parent != null; obj = parent, parent = objects.parent[obj])
            follow(parent, joint[obj]);
    }
} // namespace tnt::doo