// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <execution>

#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Sprites.hpp"
#include "json/JsonRectangle.hpp"
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
            accel.reserve(10);

            physics_queue.reserve(10);

            bound_box.reserve(10);
        }

        physics_queue.emplace_back(id);

        inv_mass.emplace_back(if_else(body.type == body_type::fixed, 0.f, 1 / body.mass));

        if (body.bound_box != Rectangle{0, 0, 0, 0})
            bound_box.emplace_back(body.bound_box + objects.pos[id]);
        else
            bound_box.emplace_back(
                objects.pos[id],
                objects.scale[id].x * sprites.clip[id].w,
                objects.scale[id].y * sprites.clip[id].h);

        damping.emplace_back(body.damping);
        restitution.emplace_back(body.restitution);

        vel.emplace_back(VECTOR_ZERO);
        accel.emplace_back(VECTOR_ZERO);
    }

    void physics_sys::add_invalid()
    {
        [[unlikely]] if (inv_mass.size() == inv_mass.capacity())
        {
            inv_mass.reserve(10);

            damping.reserve(10);
            restitution.reserve(10);

            vel.reserve(10);
            accel.reserve(10);

            physics_queue.reserve(10);
            bound_box.reserve(10);
        }

        inv_mass.emplace_back(0.f);

        damping.emplace_back(0.f);
        restitution.emplace_back(0.f);

        vel.emplace_back(VECTOR_ZERO);
        accel.emplace_back(VECTOR_ZERO);

        physics_queue.emplace_back(-1);
        bound_box.emplace_back(0.f, 0.f, 0.f, 0.f);
    }

    void physics_sys::addForce(object const &id, Vector const &force) noexcept
    {
        accel[id] += (force * inv_mass[id]);
    }

    void physics_sys::addGlobalForce(Vector const &force) noexcept
    {
        totalForce += force;
        std::for_each(
            std::execution::par_unseq,
            objects.active.cbegin(), objects.active.cend(),
            [this, &force](object const &id) noexcept -> void {
                accel[id] += (totalForce * inv_mass[id]);
            });
    }

    void physics_sys::Update(object const &id, float time_) noexcept
    {
        check(time_ > 0.f, "Calling tnt::doo::physics_sys::Update with parameter time_ equal to 0. Objects will not be updated!!");
        float const &damp{std::powf(damping[id], time_)};

        objects.pos[id] += (physics.vel[id] * time_);
        bound_box[id] += (physics.vel[id] * time_);

        vel[id] += (accel[id] * time_);
        vel[id] *= damp;
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
        Vector const &normal{[&id, &id2]() -> Vector {
            Vector const &n{objects.pos[id2] - objects.pos[id]};
            return n.Normalized();
        }()};

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
    }

    void physics_sys::from_json(nlohmann::json const &j)
    {
        if (json_has(j, "phys"))
        {
            if (nlohmann::json const &phys{j["phys"]}; json_has(phys, "bounds"))
                add_object(physics_comp{.mass{phys["mass"]}, .damping{phys["damping"]}, .restitution{phys["restitution"]}, .bound_box{phys["bounds"]}});
            else
                add_object(physics_comp{.mass{phys["mass"]}, .damping{phys["damping"]}, .restitution{phys["restitution"]}});
        }
        else
            add_invalid();
    }
} // namespace tnt::doo