// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Steering.hpp"

#include "utils/TypeUtils.hpp"

namespace tnt::doo
{
    void steering_sys::Seek(object const &id, Vector const &target) const noexcept
    {
        [[likely]] if (has_object(physics.physics_queue, id))
        {
            physics.addForce(id, (target - objects.gPos(id)).Normalized() *
                                         physics.gMaxVel(id).Magnitude() -
                                     physics.gVel(id));
            physics.vel[id] = if_else(physics.vel[id] > physics.maxVel[id],
                                      physics.maxVel[id], physics.vel[id]);
        }
    }

    void steering_sys::Flee(object const &id, Vector const &target) const noexcept
    {
        [[likely]] if (has_object(physics.physics_queue, id))
        {
            physics.addForce(id, (objects.gPos(id) - target).Normalized() *
                                         physics.gMaxVel(id).Magnitude() -
                                     physics.gVel(id));
            physics.vel[id] = if_else(physics.vel[id] > physics.maxVel[id],
                                      physics.maxVel[id], physics.vel[id]);
        }
    }

    void steering_sys::RangedFlee(object const &id, Vector const &target, float range) const noexcept
    {
        [[likely]] if (has_object(physics.physics_queue, id))
        {
            float const &dstSquared{range * range};
            if (Vector const &deltaPos{objects.gPos(id) - target};
                (deltaPos - target).MagnitudeSqr() - dstSquared > FLT_EPSILON)
            {
                physics.addForce(id, deltaPos.Normalized() * physics.gMaxVel(id).Magnitude() -
                                         physics.gVel(id));
                physics.vel[id] = if_else(physics.vel[id] > physics.maxVel[id],
                                          physics.maxVel[id], physics.vel[id]);
            }
        }
    }

    void steering_sys::Arrive(object const &id, Vector const &target) const noexcept
    {
        [[likely]] if (has_object(physics.physics_queue, id))
        {
            Vector const &to_target(target - objects.gPos(id));
            if (float const &dst{to_target.Magnitude()}; dst)
            {
                constexpr float decel_tweak{.3f};
                float const &speed{dst / (2 * decel_tweak)};
                Vector const &vel{to_target * speed * 2 * decel_tweak};

                physics.addForce(id, vel - physics.gVel(id));
                physics.vel[id] = if_else(physics.vel[id] > physics.maxVel[id],
                                          physics.maxVel[id], physics.vel[id]);
            }
        }
    }

    void steering_sys::Pursuit(object const &id, object const &target) const noexcept
    {
        [[likely]] if (has_object(physics.physics_queue, id))
        {
            [[unlikely]] if (!has_object(physics.physics_queue, target))
                Seek(id, objects.gPos(target));
            else [[likely]]
            {
                if (Vector const &to_target{objects.gPos(target) - objects.gPos(id)};
                    Dot(to_target, physics.gVel(id).Normalized()) > 0)
                    Seek(id, objects.gPos(target));
                else
                {
                    float const &look_ahead_time{to_target.Magnitude() /
                                                 (physics.gMaxVel(id).Magnitude() + physics.gVel(target).Magnitude())};
                    Seek(id, objects.gPos(target) + physics.gVel(target) * look_ahead_time);
                }
            }
        }
    }

    void steering_sys::Evade(object const &id, object const &target) const noexcept
    {
        Vector const &to_target{objects.gPos(target) - objects.gPos(id)};
        if (constexpr float range{100.f};
            to_target.Magnitude() - (range * range) < FLT_EPSILON)
        {
            float const &look_ahead_time{to_target.Magnitude() /
                                         (physics.gMaxVel(id).Magnitude() + physics.gVel(target).Magnitude())};
            Flee(id, objects.gPos(target) + physics.gVel(target) * look_ahead_time);
        }
    }
} // namespace tnt::doo