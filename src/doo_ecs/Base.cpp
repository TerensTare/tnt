// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "doo_ecs/Base.hpp"
#include "json/JsonVector.hpp"
#include "utils/TypeUtils.hpp"

namespace tnt::doo
{
    constexpr object_data::object_data(float angle_, Vector const &pos_,
                                       Vector const &scale_, object const &parent_) noexcept
        : angle{angle_}, pos{pos_}, scale{scale_}, parent{parent_} {}

    object objects_sys::add_object(object_data const &data_) noexcept
    {
        object const &index{angle.size()};

        [[unlikely]] if (index == angle.capacity()) // if the vector got to his capacity, then we reserve more space
        {
            angle.reserve(10);
            active.reserve(10);
            parent.reserve(10);
            scale.reserve(10);
            pos.reserve(10);
        }

        active.emplace_back(index);

        if (data_.parent == -1)
        {
            parent.emplace_back(-1);
            angle.emplace_back(data_.angle);
            scale.emplace_back(data_.scale);
            pos.emplace_back(data_.pos);
        }
        else
        {
            parent.emplace_back(data_.parent);
            angle.emplace_back(data_.angle - gAngle(data_.parent));
            scale.emplace_back(data_.scale - gScale(data_.parent));
            pos.emplace_back(data_.pos - gPos(data_.parent));
        }

        return index;
    }

    [[nodiscard]] object_data objects_sys::get_data(object const &id) const noexcept
    {
        return object_data{gAngle(id), gPos(id), gScale(id), parent[id]};
    }

    void objects_sys::from_json(nlohmann::json const &j)
    {
        if (json_has(j, "parent"))
            add_object({j["angle"].get<float>(),
                        j["pos"].get<Vector>(),
                        j["scale"].get<Vector>(),
                        j["parent"].get<object>()});
        else
            add_object({j["angle"].get<float>(),
                        j["pos"].get<Vector>(),
                        j["scale"].get<Vector>()});
    }

    float objects_sys::gAngle(object const &id) const noexcept
    {
        return angle[id] + if_then(parent[id] != -1,
                                   angle[parent[id]]);
    }

    Vector objects_sys::gScale(object const &id) const noexcept
    {
        if (parent[id] == -1)
            return scale[id];
        Vector const &globScale{gScale(parent[id])};
        return Vector{globScale.x * scale[id].x, globScale.y * scale[id].y};
    }

    Vector objects_sys::gPos(object const &id) const noexcept
    {
        if (parent[id] == -1)
            return pos[id];

        Vector const &pScale{scale[parent[id]]};
        Vector const &rotPos{RotateVector({pos[id].x * pScale.x, pos[id].y * pScale.y}, angle[parent[id]])};
        return gPos(parent[id]) + rotPos;
    }

    void objects_sys::set_parent(object const &id, object const &parent_) noexcept
    {
        if (parent_ == -1)
        {
            angle[id] = gAngle(id);
            scale[id] = gScale(id);
            pos[id] = gPos(id);
        }
        else
        {
            if (parent[id] != -1)
                set_parent(id, -1); // remove the current parent

            Vector const &pScale{gScale(parent_)};
            pos[id] = RotateVector(gPos(id) - gPos(parent_), -gAngle(parent_));
            pos[id].x /= pScale.x;
            pos[id].y /= pScale.y;

            angle[id] -= gAngle(parent_);
            scale[id] = Vector{scale[id].x / pScale.x, scale[id].y / pScale.y};
        }

        parent[id] = parent_;
    }
} // namespace tnt::doo