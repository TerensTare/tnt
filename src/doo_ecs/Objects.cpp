// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "doo_ecs/Objects.hpp"
#include "json/JsonVector.hpp"
#include "imgui/ImGui.hpp"

namespace tnt::doo
{
    inline static bool contains(nlohmann::json const &j, const char *value) noexcept
    {
        return j.find(value) != j.end();
    }

    inline static constexpr auto local_scale = [](Vector const &myScale, Vector const &pScale) noexcept -> Vector {
        return {myScale.x / pScale.x, myScale.y / pScale.y};
    };

    inline static constexpr auto local_pos = [](Vector const &myPos, float pAngle, Vector const &pScale, Vector const &pPos) noexcept -> Vector {
        Vector const &diff{RotateVector(myPos - pPos, -pAngle)};
        return local_scale(diff, pScale);
    };

    struct gen final
    {
        inline static object next() noexcept
        {
            static object id_{(object)0};
            return id_++;
        }
    };

    object objects_sys::add_object(object_data const &data_) noexcept
    {
        object const &index{gen::next()};

        [[unlikely]] if (angle.size() == angle.capacity()) // if the vector got to his capacity, then we reserve more space
        {
            angle.reserve(10);
            parent.reserve(10);
            scale.reserve(10);
            pos.reserve(10);
        }

        active.push_back(index);

        parent.emplace_back(null);
        angle.emplace_back(data_.angle);
        scale.emplace_back(data_.scale);
        pos.emplace_back(data_.pos);

        set_parent(index, data_.parent);

        return index;
    }

    [[nodiscard]] object_data objects_sys::get_data(object const &id) const noexcept
    {
        return object_data{gAngle(id), gPos(id), gScale(id), parent[id]};
    }

    object objects_sys::from_json(nlohmann::json const &j)
    {
        object id{null};
        if (contains(j, "parent"))
            id = add_object({j["angle"].get<float>(),
                             j["pos"].get<Vector>(),
                             j["scale"].get<Vector>(),
                             j["parent"].get<object>()});
        else
            id = add_object({j["angle"].get<float>(),
                             j["pos"].get<Vector>(),
                             j["scale"].get<Vector>()});
        return id;
    }

    void objects_sys::draw_imgui(object const &id, Window const &win) noexcept
    {
        tnt::ImGui::hslider_float(win, "Angle", -360.f, 360.f, &objects.angle[id]);
        tnt::ImGui::hslider_vec(win, "Scale", .1f, 20.f, .1f, 20.f, &objects.scale[id]);
        // TODO: rethink this (maybe) substract (gPos(id) - pos[id]) ??
        tnt::ImGui::hslider_vec(win, "Pos", 0.f, win.getWidth() * 1.f,
                                0.f, win.getHeight() * 1.f,
                                &objects.pos[id]);
    }

    float objects_sys::gAngle(object const &id) const noexcept
    {
        if (parent[id] == null)
            return angle[id];
        return angle[id] + gAngle(parent[id]);
    }

    Vector objects_sys::gScale(object const &id) const noexcept
    {
        if (parent[id] == null)
            return scale[id];
        Vector const &globScale{gScale(parent[id])};
        return Vector{globScale.x * scale[id].x, globScale.y * scale[id].y};
    }

    Vector objects_sys::gPos(object const &id) const noexcept
    {
        if (parent[id] == null)
            return pos[id];

        Vector const &pScale{gScale(parent[id])};
        Vector const &rotPos{RotateVector({pos[id].x * pScale.x, pos[id].y * pScale.y}, angle[parent[id]])};
        return gPos(parent[id]) + rotPos;
    }

    void objects_sys::set_parent(object const &id, object const &parent_) noexcept
    {
        if (parent_ == null)
        {
            angle[id] = gAngle(id);
            scale[id] = gScale(id);
            pos[id] = gPos(id);
        }
        else
        {
            if (parent[id] != null)
                set_parent(id, null); // remove the current parent

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