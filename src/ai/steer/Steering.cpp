// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ai/steer/Steering.hpp"
#include "ecs/RigidBody.hpp"

tnt::Vector tnt::steer::Seek(Object const *obj, Vector const &target)
{
    Vector ret{0.f, 0.f};
    [[likely]] if (obj->has<PhysicsComponent>())
    {
        Vector velocity{target - obj->getPosition()};
        velocity.Normalize();
        const PhysicsComponent *phys{obj->get<PhysicsComponent>()};
        velocity *= phys->getMaxSpeed();
        ret = (velocity - phys->getVelocity()) * phys->getMass();
    }

    return ret;
}

tnt::Vector tnt::steer::Flee(Object const *obj, Vector const &target)
{
    Vector ret{0.f, 0.f};
    [[likely]] if (obj->has<PhysicsComponent>())
    {
        Vector velocity{obj->getPosition() - target};
        velocity.Normalize();
        const PhysicsComponent *phys{obj->get<PhysicsComponent>()};
        velocity *= phys->getMaxSpeed();
        ret = (velocity - phys->getVelocity()) * phys->getMass();
    }

    return ret;
}

tnt::Vector tnt::steer::RangedFlee(Object const *obj, Vector const &target, float range)
{
    const float dstSquared{range * range};
    Vector ret{0.f, 0.f};
    [[likely]] if (obj->has<PhysicsComponent>())
    {
        ret = obj->getPosition() - target;
        if ((ret - target).MagnitudeSqr() - dstSquared > FLT_EPSILON)
        {
            Vector velocity{obj->getPosition() - target};
            velocity.Normalize();
            const PhysicsComponent *phys{obj->get<PhysicsComponent>()};
            velocity *= phys->getVelocity().Magnitude();
            ret = (velocity - phys->getVelocity()) * phys->getMass();
        }
    }

    return ret;
}

tnt::Vector tnt::steer::Arrive(Object const *obj, Vector const &target, int decel)
{
    Vector ret{0.f, 0.f};
    [[likely]] if (obj->has<PhysicsComponent>())
    {
        const Vector toTarget{target - obj->getPosition()};

        [[likely]] if (const float dst{toTarget.Magnitude()};
                       dst > FLT_EPSILON)
        {
            const float decelTweak{.3f};
            const float speed{dst / (decel * decelTweak)};
            const Vector velocity{toTarget * (speed / dst)};
            const PhysicsComponent *phys{obj->get<PhysicsComponent>()};
            ret = (velocity - phys->getVelocity()) * phys->getMass();
        }
    }
    return ret;
}

tnt::Vector tnt::steer::Pursuit(Object const *obj, Object const *target)
{
    Vector ret{0.f, 0.f};
    [[likely]] if (obj->has<PhysicsComponent>())
    {
        [[unlikely]] if (!target->has<PhysicsComponent>())
        {
            ret = Seek(obj, target->getPosition());
        }
        else [[likely]]
        {
            const Vector toTarget{target->getPosition() - obj->getPosition()};
            const PhysicsComponent *phys{obj->get<PhysicsComponent>()};
            const PhysicsComponent *phys2{target->get<PhysicsComponent>()};
            const Vector relHeading{Dot(phys->getVelocity().Normalized(), phys2->getVelocity().Normalized())};
            if (Dot(toTarget, phys->getDirection()) > FLT_EPSILON)
                ret = Seek(obj, target->getPosition());
            else
            {
                const float lookAheadTime{
                    toTarget.Magnitude() /
                    (phys->getMaxSpeed() + phys2->getSpeed())};
                ret = Seek(obj, target->getPosition() + phys2->getVelocity() * lookAheadTime);
            }
        }
    }

    return ret;
}

tnt::Vector tnt::steer::Evade(Object const *obj, Object const *target)
{
    Vector ret{0.f, 0.f};

    [[likely]] if (obj->has<PhysicsComponent>())
    {
        [[likely]] if (target->has<PhysicsComponent>())
        {
            const Vector toTarget{target->getPosition() - obj->getPosition()};
            const PhysicsComponent *phys{obj->get<PhysicsComponent>()};
            const PhysicsComponent *phys2{target->get<PhysicsComponent>()};
            const float lookAheadTime{
                toTarget.Magnitude() /
                (phys->getMaxSpeed() + phys2->getSpeed())};
            ret = Flee(obj, target->getPosition() +
                                (phys2->getVelocity() * lookAheadTime));
        }
        else [[unlikely]] ret = Flee(obj, target->getPosition());
    }

    return ret;
}