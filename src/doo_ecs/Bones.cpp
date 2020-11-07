// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <nlohmann/json.hpp>

#include "doo_ecs/Bones.hpp"
#include "doo_ecs/Objects.hpp"

#include "utils/Assert.hpp"
#include "utils/Containers.hpp"

namespace tnt::doo
{
    inline const auto bottom = [](object const &id, bone_data const &data_) noexcept -> Vector {
        Vector const &gPos{objects.gPos(id)};
        float const &gAngle{objects.gAngle(id)};
        Vector const &lEnd{data_.length * cosf(gAngle),
                           data_.length * sinf(gAngle)};
        return gPos + lEnd;
    };

    bone_tree::bone_tree(bone_data const &data_)
    {
        if (length.size() == length.capacity())
        {
            length.reserve(10);
            minAngle.reserve(10);
            maxAngle.reserve(10);
            ids.reserve(10);
            parents.reserve(10);
        }

        length.emplace_back(data_.length);
        minAngle.emplace_back(data_.minAngle);
        maxAngle.emplace_back(data_.maxAngle);
        ids.emplace_back(length.size() - 1);
        parents.emplace_back(-1);
    }

    void bone_tree::add_bone(object const &parent_, bone_data const &data_)
    {
        if (length.size() == length.capacity())
        {
            length.reserve(10);
            minAngle.reserve(10);
            maxAngle.reserve(10);
            ids.reserve(10);
            parents.reserve(10);
        }

        length.emplace_back(data_.length);
        minAngle.emplace_back(data_.minAngle);
        maxAngle.emplace_back(data_.maxAngle);
        ids.emplace_back(length.size() - 1);
        parents.emplace_back(parent_);
    }

    void bones_sys::add_object(bone_data const &data_)
    {
        object const &id{active.size()};
        if (id == active.capacity())
        {
            active.reserve(10);
            trees.reserve(10);
        }

        active.emplace_back(id);
        trees.emplace_back(data_);
    }

    void bones_sys::add_invalid()
    {
        if (active.size() == active.capacity())
        {
            active.reserve(10);
            trees.reserve(10);
        }

        active.emplace_back(-1);
        trees.emplace_back(bone_data{.length{0.f}, .minAngle{0.f}, .maxAngle{0.f}});
    }

    void bones_sys::from_json(nlohmann::json const &j)
    {
        if (contains(j, "bones"))
        {
            nlohmann::json const &b{j["bones"]};
            float min{0.f}, max{360.f};
            if (contains(b, "minAngle"))
                min = b["minAngle"];
            if (contains(b, "maxAngle"))
                max = b["maxAngle"];
            add_object(bone_data{.length{b["length"]}, .minAngle{min}, .maxAngle{max}});
        }
        else
            add_invalid();
    }
} // namespace tnt::doo