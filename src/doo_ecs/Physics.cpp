// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "doo_ecs/Objects.hpp"
#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Sprites.hpp"

#include "json/JsonRectangle.hpp"
#include "json/JsonVector.hpp"

#include "imgui/ImGui.hpp"

#include "utils/Assert.hpp"

namespace tnt::doo
{
    inline static bool contains(nlohmann::json const &j, const char *value) noexcept
    {
        return j.find(value) != j.end();
    }

    void physics_sys::add_object(object const &id, physics_comp const &body)
    {
        [[unlikely]] if (id > inv_mass.capacity())
        {
            inv_mass.reserve(id - inv_mass.capacity());

            damping.reserve(id - inv_mass.capacity());
            restitution.reserve(id - inv_mass.capacity());

            vel.reserve(id - inv_mass.capacity());
            maxVel.reserve(id - inv_mass.capacity());
            accel.reserve(id - inv_mass.capacity());
            maxAccel.reserve(id - inv_mass.capacity());

            bound_box.reserve(id - inv_mass.capacity());
        }

        inv_mass.emplace(inv_mass.cbegin() + id, if_else(body.type == body_type::fixed, 0.f, 1 / body.mass));

        damping.emplace(damping.cbegin() + id, body.damping);
        restitution.emplace(restitution.cbegin() + id, body.restitution);

        if (objects.parent[id] == null)
        {
            vel.emplace(vel.cbegin() + id, VECTOR_ZERO);
            maxVel.emplace(maxVel.cbegin() + id, body.maxVel);
            accel.emplace(accel.cbegin() + id, VECTOR_ZERO);
            maxAccel.emplace(maxAccel.cbegin() + id, body.maxAccel);
        }
        else
        {
            vel.emplace(vel.cbegin() + id, -gVel(objects.parent[id]));
            maxVel.emplace(maxVel.cbegin() + id, body.maxVel - gMaxVel(objects.parent[id]));
            accel.emplace(accel.cbegin() + id, -gAccel(objects.parent[id]));
            maxAccel.emplace(maxAccel.cbegin() + id, body.maxAccel - gMaxAccel(objects.parent[id]));
        }

        active.insert(id, id);

        if (body.bound_box != Rectangle{0, 0, 0, 0})
            bound_box.emplace(bound_box.cbegin() + id, body.bound_box + objects.gPos(id));
        else
            bound_box.emplace(bound_box.begin() + id,
                              objects.gPos(id),
                              objects.gScale(id).x * sprites.clip[id].w,
                              objects.gScale(id).y * sprites.clip[id].h);
    }

    void physics_sys::addForce(object const &id, Vector const &force) noexcept
    {
        accel[id] += (force * inv_mass[id]);
        accel[id] = if_else(accel[id] > maxAccel[id], maxAccel[id], accel[id]);
    }

    void physics_sys::addGlobalForce(Vector const &force) noexcept
    {
        totalForce += force;
        for (auto const &id : active)
            addForce(id, totalForce);
    }

    void physics_sys::Update(object const &id, float time_) noexcept
    {
        if (active.contains(id))
        {
            check(time_ > 0.f, "Calling tnt::doo::physics_sys::Update with parameter time_ equal to 0. Objects will not be updated!!");
            float const &damp{std::powf(damping[id], time_)};

            objects.pos[id] += (physics.vel[id] * time_);
            bound_box[id] += (physics.vel[id] * time_);

            vel[id] += (accel[id] * time_);
            vel[id] *= damp;
            vel[id] = if_else(vel[id] > maxVel[id], maxVel[id], vel[id]);
        }
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

    void physics_sys::resolveVel(object const &id, object const &id2) noexcept
    {
        if (active.contains(id) && active.contains(id2))
        {
            Vector const &normal{(objects.gPos(id) - objects.gPos(id2)).Normalized()};

            float const &sep_vel = [this, &normal](object const &id, object const &id2) noexcept -> float {
                Vector const &rel_vel{gVel(id) - gVel(id2)};
                return Dot(rel_vel, normal);
            }(id, id2);

            if (sep_vel > 0.f)
                return;

            float const &res{if_else(restitution[id] < restitution[id2],
                                     restitution[id], restitution[id2])};

            float new_sep_vel{-sep_vel * res};
            Vector const &acc_caused_vel{gAccel(id) - gAccel(id2)};

            if (float const &acc_caused_sep_vel{
                    Dot(acc_caused_vel, normal)};
                acc_caused_sep_vel < 0.f)
            {
                new_sep_vel += res * acc_caused_sep_vel;
                if (new_sep_vel < 0.f)
                    new_sep_vel = 0.f;
            }

            float const &delta_vel{new_sep_vel - sep_vel};
            float const &total_inv_mass{inv_mass[id] + inv_mass[id2]};
            float const &impulse{delta_vel / total_inv_mass};
            Vector const &impulse_per_mass{normal * impulse};

            vel[id] += impulse_per_mass * inv_mass[id];
            vel[id] = if_else(vel[id] > maxVel[id],
                              maxVel[id], vel[id]);

            vel[id2] -= impulse_per_mass * inv_mass[id2];
            vel[id2] = if_else(vel[id2] > maxVel[id2],
                               maxVel[id2], vel[id2]);
        }
    }

    void physics_sys::resolveInterpenetration(object const &id, object const &id2) noexcept
    {
    }

    void physics_sys::resolve(object const &id, object const &id2) noexcept
    {
        resolveVel(id, id2);
        resolveInterpenetration(id, id2);
    }

    void physics_sys::from_json(object const &id, nlohmann::json const &j)
    {
        if (contains(j, "phys"))
        {
            if (nlohmann::json const &phys{j["phys"]};
                contains(phys, "bounds"))
            {
                if (contains(phys, "max_vel") || contains(phys, "max_accel"))
                    add_object(id, {.mass{phys["mass"]},
                                    .damping{phys["damping"]},
                                    .restitution{phys["restitution"]},
                                    .maxVel = phys["max_vel"],
                                    .maxAccel = phys["max_accel"],
                                    .bound_box{phys["bounds"]}});
                else
                    add_object(id, {.mass{phys["mass"]},
                                    .damping{phys["damping"]},
                                    .restitution{phys["restitution"]},
                                    .bound_box{phys["bounds"]}});
            }
            else
            {
                if (contains(phys, "max_vel") || contains(phys, "max_accel"))
                    add_object(id, {.mass{phys["mass"]},
                                    .damping{phys["damping"]},
                                    .restitution{phys["restitution"]},
                                    .maxVel = phys["max_vel"],
                                    .maxAccel = phys["max_accel"]});
                else
                    add_object(id, {.mass{phys["mass"]},
                                    .damping{phys["damping"]},
                                    .restitution{phys["restitution"]}});
            }
        }
    }

    void physics_sys::draw_imgui(object const &id, Window const &win) noexcept
    {
        if (active.contains(id))
            if (tnt::ImGui::BeginSection(win, "Physics"))
            {
                tnt::ImGui::hslider_float(win, "Damping", 0.f, 1.f, &physics.damping[id]);
                tnt::ImGui::hslider_float(win, "Restitution", 0.f, 1.f, &physics.restitution[id]);

                tnt::ImGui::EndSection();
            }
    }

    Vector physics_sys::gVel(object const &id) const noexcept
    {
        if (objects.parent[id] == null)
            return vel[id];
        Vector const &parent_vel{gVel(objects.parent[id])};
        return parent_vel + RotateVector(vel[id], AngleOf(parent_vel) - AngleOf(vel[id]));
    }

    Vector physics_sys::gMaxVel(object const &id) const noexcept
    {
        if (objects.parent[id] == null)
            return maxVel[id];
        Vector const &parent_vel{gMaxVel(objects.parent[id])};
        return parent_vel + RotateVector(maxVel[id], AngleOf(parent_vel) - AngleOf(maxVel[id]));
    }

    Vector physics_sys::gAccel(object const &id) const noexcept
    {
        if (objects.parent[id] == null)
            return accel[id];
        Vector const &parent_accel{gAccel(objects.parent[id])};
        return parent_accel + RotateVector(accel[id], AngleOf(parent_accel) - AngleOf(accel[id]));
    }

    Vector physics_sys::gMaxAccel(object const &id) const noexcept
    {
        if (objects.parent[id] == null)
            return maxAccel[id];
        Vector const &parent_accel{gMaxAccel(objects.parent[id])};
        return parent_accel + RotateVector(maxAccel[id], AngleOf(parent_accel) - AngleOf(maxAccel[id]));
    }
} // namespace tnt::doo