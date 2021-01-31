// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "doo_ecs/Objects.hpp"
#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Sprites.hpp"

#include "json/JsonRectangle.hpp"
#include "json/JsonVector.hpp"

#include "imgui/ImGui.hpp"

#include "utils/Assert.hpp"
#include "utils/Benchmark.hpp"

namespace tnt::doo
{
    void physics_sys::add_object(object const &id, physics_comp const &body)
    {
        safe_ensure(objects.active.contains(id), "Adding inexistent object to animations_sys!!");
        PROFILE_FUNCTION();

        [[unlikely]] if (id > inv_mass.capacity())
        {
            std::size_t const diff{id - inv_mass.capacity()};

            inv_mass.reserve(diff);

            damping.reserve(diff);
            restitution.reserve(diff);

            vel.reserve(diff);
            maxVel.reserve(diff);
            accel.reserve(diff);
            maxAccel.reserve(diff);
            force.reserve(diff);

            bound_box.reserve(diff);
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
        force.emplace_back();

        active.insert(id, id);

        if (body.bound_box != Rectangle{0, 0, 0, 0})
            bound_box.emplace(bound_box.cbegin() + id, body.bound_box + objects.gPos(id));
        else
            bound_box.emplace(bound_box.begin() + id,
                              objects.gPos(id),
                              objects.gScale(id).x * sprites.clip[id].w,
                              objects.gScale(id).y * sprites.clip[id].h);
    }

    void physics_sys::addForce(object const &id, Vector const &force_) noexcept
    {
        force[id] += force_;
    }

    void physics_sys::addGlobalForce(Vector const &force_) noexcept
    {
        totalForce += force_;
    }

    void physics_sys::Update(object const &id, float time_) noexcept
    {
        if (active.contains(id))
        {
            PROFILE_FUNCTION();

            check(time_ > 0.f, "Calling tnt::doo::physics_sys::Update with parameter time_ equal to 0. Objects will not be updated!!");
            float const &damp{std::powf(damping[id], time_)};

            force[id] += totalForce;

            objects.pos[id] += (vel[id] * time_);
            bound_box[id] += (vel[id] * time_);

            vel[id] += (accel[id] * time_);
            vel[id] *= damp;
            vel[id] = std::min(maxVel[id], vel[id]);

            accel[id] += (force[id] * inv_mass[id]);
            accel[id] = std::min(maxAccel[id], accel[id]);

            force[id] = VECTOR_ZERO;
        }
    }

    bool physics_sys::colliding(object const &id, object const &id2) noexcept
    {
        PROFILE_FUNCTION();

        // TODO: check for collision when angle is non-zero.
        bool ret = !(bound_box[id2].x - (bound_box[id].x + bound_box[id].w) > 0.f ||
                     bound_box[id2].y - (bound_box[id].y + bound_box[id].h) > 0.f);
        ret = ret && !(bound_box[id].x - (bound_box[id2].x + bound_box[id2].w) > 0.f ||
                       bound_box[id].y - (bound_box[id2].y + bound_box[id2].h) > 0.f);

        return ret;
    }

    void physics_sys::resolve(object const &id, object const &id2) noexcept
    {
        if (active.contains(id) && active.contains(id2))
        {
            PROFILE_FUNCTION();

            Vector const &normal{(objects.gPos(id) - objects.gPos(id2)).Normalized()};

            float const &sep_vel = [this, &normal](object const &id, object const &id2) noexcept -> float {
                Vector const &rel_vel{gVel(id2) - gVel(id)};
                return Dot(rel_vel, normal);
            }(id, id2);

            if (sep_vel > 0.f)
                return;

            float const &res{std::min(restitution[id], restitution[id2])};

            float j{-(1.f + res) * sep_vel};
            j /= (inv_mass[id] + inv_mass[id2]);

            float const &total_mass{1.f / inv_mass[id] + 1.f / inv_mass[id2]};

            Vector const &impulse{normal * j};
            vel[id] -= impulse * (total_mass / inv_mass[id]);
            vel[id] = std::min(maxVel[id], vel[id]);

            vel[id2] += impulse * (total_mass / inv_mass[id2]);
            vel[id2] = std::min(maxVel[id2], vel[id2]);

            // TODO:
            // apply positional correction using slop
            float pen{0.f};

            Vector const &n{objects.pos[id2] - objects.pos[id]};
            float const &x_overlap{bound_box[id].w + bound_box[id2].w - std::fabsf(n.x)};
            if (x_overlap > 0.f)
            {
                float const &y_overlap{bound_box[id].h + bound_box[id2].h - std::fabsf(n.y)};
                if (y_overlap > 0.f)
                    pen = std::max(x_overlap, y_overlap);
            }

            Vector const &correction{std::max(pen - phys::slop, 0.f) / (inv_mass[id] + inv_mass[id2])};
            objects.pos[id] -= inv_mass[id] * correction;
            objects.pos[id2] += inv_mass[id2] * correction;
        }
    }

    void physics_sys::from_json(object const &id, nlohmann::json const &j)
    {
        if (j.contains("phys"))
        {
            PROFILE_FUNCTION();

            if (nlohmann::json const &phys{j["phys"]};
                phys.contains("bounds"))
            {
                if (phys.contains("max_vel") || phys.contains("max_accel"))
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
                if (phys.contains("max_vel") || phys.contains("max_accel"))
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

    void physics_sys::to_json(object const &id, nlohmann::json &j)
    {
        if (active.contains(id))
        {
            PROFILE_FUNCTION();

            nlohmann::json &p{j["phys"]};
            p["mass"] = 1 / inv_mass[id];
            p["damping"] = damping[id];
            p["restitution"] = restitution[id];
            p["max_vel"] = gMaxVel(id);
            p["max_accel"] = gMaxAccel(id);
            p["bounds"] = bound_box[id];
        }
    }

    void physics_sys::remove(object const &id) noexcept
    {
        PROFILE_FUNCTION();

        active.erase(id);
        inv_mass.erase(inv_mass.cbegin() + id);
        damping.erase(damping.cbegin() + id);
        restitution.erase(restitution.cbegin() + id);

        vel.erase(vel.cbegin() + id);
        maxVel.erase(maxVel.cbegin() + id);
        accel.erase(accel.cbegin() + id);
        maxAccel.erase(maxAccel.cbegin() + id);
        force.erase(force.cbegin() + id);

        bound_box.erase(bound_box.cbegin() + id);
    }

    void physics_sys::clear() noexcept
    {
        PROFILE_FUNCTION();

        active.clear();
        inv_mass.clear();
        damping.clear();
        restitution.clear();

        vel.clear();
        maxVel.clear();
        accel.clear();
        maxAccel.clear();
        force.clear();

        bound_box.clear();
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