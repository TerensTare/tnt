// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Steering.hpp"

#include "utils/TypeUtils.hpp"

namespace tnt::doo
{
    void steering_sys::Seek(object const &id, Vector const &target) const noexcept
    {
        physics.addForce(id, (target - objects.gPos(id)).Normalized() *
                                     physics.gMaxVel(id).Magnitude() -
                                 physics.gVel(id));
        physics.vel[id] = if_else(physics.vel[id] > physics.maxVel[id],
                                  physics.maxVel[id], physics.vel[id]);
    }

    void steering_sys::Flee(object const &id, Vector const &target) const noexcept
    {
        physics.addForce(id, (objects.gPos(id) - target).Normalized() *
                                     physics.gMaxVel(id).Magnitude() -
                                 physics.gVel(id));
        physics.vel[id] = if_else(physics.vel[id] > physics.maxVel[id],
                                  physics.maxVel[id], physics.vel[id]);
    }

    void steering_sys::RangedFlee(object const &id, Vector const &target, float range) const noexcept
    {
        float const &dstSquared{range * range};
        Vector const &deltaPos{objects.gPos(id) - target};
        if ((deltaPos - target).MagnitudeSqr() - dstSquared > FLT_EPSILON)
        {
            physics.addForce(id, deltaPos.Normalized() * physics.gMaxVel(id).Magnitude() -
                                     physics.gVel(id));
            physics.vel[id] = if_else(physics.vel[id] > physics.maxVel[id],
                                      physics.maxVel[id], physics.vel[id]);
        }
    }
} // namespace tnt::doo