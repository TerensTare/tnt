// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <execution>

#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Sprites.hpp"

#include "json/JsonRectangle.hpp"
#include "json/JsonVector.hpp"

#include "imgui/ImGui.hpp"

#include "utils/Assert.hpp"

namespace tnt::doo
{
    void physics_sys::add_object(physics_comp const &body)
    {
        object const &id{inv_mass.size()};
        [[unlikely]] if (id == inv_mass.capacity())
        {
            inv_mass.reserve(10);

            damping.reserve(10);
            restitution.reserve(10);

            vel.reserve(10);
            maxVel.reserve(10);
            accel.reserve(10);
            maxAccel.reserve(10);

            physics_queue.reserve(10);

            bound_box.reserve(10);
        }

        inv_mass.emplace_back(if_else(body.type == body_type::fixed, 0.f, 1 / body.mass));

        damping.emplace_back(body.damping);
        restitution.emplace_back(body.restitution);

        if (objects.parent[id] == -1)
        {
            vel.emplace_back(VECTOR_ZERO);
            maxVel.emplace_back(body.maxVel);
            accel.emplace_back(VECTOR_ZERO);
            maxAccel.emplace_back(body.maxAccel);
        }
        else
        {
            vel.emplace_back(-gVel(objects.parent[id]));
            maxVel.emplace_back(body.maxVel - gMaxVel(objects.parent[id]));
            accel.emplace_back(-gAccel(objects.parent[id]));
            maxAccel.emplace_back(body.maxAccel - gMaxAccel(objects.parent[id]));
        }

        physics_queue.emplace_back(id);
        if (body.bound_box != Rectangle{0, 0, 0, 0})
            bound_box.emplace_back(body.bound_box + objects.gPos(id));
        else
            bound_box.emplace_back(
                objects.gPos(id),
                objects.gScale(id).x * sprites.clip[id].w,
                objects.gScale(id).y * sprites.clip[id].h);
    }

    void physics_sys::add_invalid()
    {
        [[unlikely]] if (inv_mass.size() == inv_mass.capacity())
        {
            inv_mass.reserve(10);

            damping.reserve(10);
            restitution.reserve(10);

            vel.reserve(10);
            maxVel.reserve(10);
            accel.reserve(10);
            maxAccel.reserve(10);

            physics_queue.reserve(10);
            bound_box.reserve(10);
        }

        inv_mass.emplace_back(0.f);

        damping.emplace_back(0.f);
        restitution.emplace_back(0.f);

        vel.emplace_back(VECTOR_ZERO);
        maxVel.emplace_back(40.f, 40.f);
        accel.emplace_back(VECTOR_ZERO);
        maxAccel.emplace_back(15.f, 15.f);

        physics_queue.emplace_back(-1);
        bound_box.emplace_back(0.f, 0.f, 0.f, 0.f);
    }

    void physics_sys::addForce(object const &id, Vector const &force) noexcept
    {
        accel[id] += (force * inv_mass[id]);
        accel[id] = if_else(accel[id] > maxAccel[id], maxAccel[id], accel[id]);
    }

    void physics_sys::addGlobalForce(Vector const &force) noexcept
    {
        totalForce += force;
        std::for_each(
            std::execution::par_unseq,
            physics_queue.cbegin(), physics_queue.cend(),
            [this, &force](object const &id) noexcept -> void {
                addForce(id, totalForce);
            });
    }

    void physics_sys::Update(object const &id, float time_) noexcept
    {
        if (!has_object(physics_queue, id))
            return;

        check(time_ > 0.f, "Calling tnt::doo::physics_sys::Update with parameter time_ equal to 0. Objects will not be updated!!");
        float const &damp{std::powf(damping[id], time_)};

        objects.pos[id] += (physics.vel[id] * time_);
        bound_box[id] += (physics.vel[id] * time_);

        vel[id] += (accel[id] * time_);
        vel[id] *= damp;
        vel[id] = if_else(vel[id] > maxVel[id], maxVel[id], vel[id]);
    }

    bool physics_sys::colliding(object const &id, object const &id2) noexcept
    {
        // TODO: check for collision when angle is non-zero.
        bool ret = !(bound_box[id2].x - (bound_box[id].x + bound_box[id].w) > 0.f ||
                     bound_box[id2].y - (bound_box[id].y + bound_box[id].h) > 0.f);
        ret = ret && !(bound_box[id].x - (bound_box[id2].x + bound_box[id2].w) > 0.f ||
                       bound_box[id].y - (bound_box[id2].y + bound_box[id2].h) > 0.f);

        return ret;
    }

    void physics_sys::resolve(object const &id, object const &id2) noexcept
    {
        Vector const &normal{(objects.gPos(id2) - objects.gPos(id)).Normalized()};

        Vector const &rel_vel{vel[id2] - vel[id]};
        float const &velAlongNormal{Dot(rel_vel, normal)};

        if (velAlongNormal > 0.f)
            return;

        float const &eps{if_else(restitution[id] < restitution[id2],
                                 restitution[id], restitution[id2])};
        float j{-(1 + eps) / velAlongNormal};
        j /= (inv_mass[id] + inv_mass[id2]);

        Vector const &impulse{normal * j};

        float const &totalMass{(1 / inv_mass[id]) + (1 / inv_mass[id2])};

        vel[id] -= impulse * totalMass * inv_mass[id];
        vel[id2] += impulse * totalMass * inv_mass[id2];
        vel[id2] = if_else(vel[id2] > maxVel[id2], maxVel[id2], vel[id2]);
    }

    void physics_sys::from_json(nlohmann::json const &j)
    {
        if (json_has(j, "phys"))
        {
            if (nlohmann::json const &phys{j["phys"]}; json_has(phys, "bounds"))
            {
                if (json_has(phys, "max_vel") || json_has(phys, "max_accel"))
                    add_object(physics_comp{.mass{phys["mass"]}, .damping{phys["damping"]}, .restitution{phys["restitution"]}, .maxVel = phys["max_vel"], .maxAccel = phys["max_accel"], .bound_box{phys["bounds"]}});
                else
                    add_object(physics_comp{.mass{phys["mass"]}, .damping{phys["damping"]}, .restitution{phys["restitution"]}, .bound_box{phys["bounds"]}});
            }
            else
            {
                if (json_has(phys, "max_vel") || json_has(phys, "max_accel"))
                    add_object(physics_comp{.mass{phys["mass"]}, .damping{phys["damping"]}, .restitution{phys["restitution"]}, .maxVel = phys["max_vel"], .maxAccel = phys["max_accel"]});
                else
                    add_object(physics_comp{.mass{phys["mass"]}, .damping{phys["damping"]}, .restitution{phys["restitution"]}});
            }
        }
        else
            add_invalid();
    }

    void physics_sys::draw_imgui(object const &id, Window const &win) noexcept
    {
        if (tnt::ImGui::BeginSection(win, "Physics"))
        {
            tnt::ImGui::hslider_float(win, "Damping", 0.f, 1.f, &physics.damping[id]);
            tnt::ImGui::hslider_float(win, "Restitution", 0.f, 1.f, &physics.restitution[id]);

            tnt::ImGui::EndSection();
        }
    }

    Vector physics_sys::gVel(object const &id) const noexcept
    {
        if (objects.parent[id] == -1)
            return vel[id];
        Vector const &parent_vel{gVel(objects.parent[id])};
        return parent_vel + RotateVector(vel[id], AngleOf(parent_vel) - AngleOf(vel[id]));
    }

    Vector physics_sys::gMaxVel(object const &id) const noexcept
    {
        if (objects.parent[id] == -1)
            return maxVel[id];
        Vector const &parent_vel{gMaxVel(objects.parent[id])};
        return parent_vel + RotateVector(maxVel[id], AngleOf(parent_vel) - AngleOf(maxVel[id]));
    }

    Vector physics_sys::gAccel(object const &id) const noexcept
    {
        if (objects.parent[id] == -1)
            return accel[id];
        Vector const &parent_accel{gAccel(objects.parent[id])};
        return parent_accel + RotateVector(accel[id], AngleOf(parent_accel) - AngleOf(accel[id]));
    }

    Vector physics_sys::gMaxAccel(object const &id) const noexcept
    {
        if (objects.parent[id] == -1)
            return maxAccel[id];
        Vector const &parent_accel{gMaxAccel(objects.parent[id])};
        return parent_accel + RotateVector(maxAccel[id], AngleOf(parent_accel) - AngleOf(maxAccel[id]));
    }
} // namespace tnt::doo