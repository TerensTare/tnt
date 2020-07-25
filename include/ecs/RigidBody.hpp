#ifndef TNT_RIGID_BODY_HPP
#define TNT_RIGID_BODY_HPP

#include "ecs/Object.hpp"
#include "math/Rectangle.hpp"

// TODO: find a way to apply forces to the RigidBody.
// TODO: handle collision in Update.
// TODO: move Joint on another file.

// TODO(maybe):
// Update() from outside RigidBody ??
// use a force accumulator for adding forces ??

namespace tnt
{
    class PhysicsComponent final : public Component
    {
    public:
        PhysicsComponent(float const &mass, tnt::Vector const &maxVel, tnt::Vector const &maxAccel) noexcept;

        void setMass(float const &mass);
        float getMass() const noexcept(noexcept(invMass > 0.f));

        Vector getVelocity() const noexcept;
        Vector getAcceleration() const noexcept;

        Vector getMaxVelocity() const noexcept;
        Vector getMaxAcceleration() const noexcept;

        Vector getDirection() const;
        float getSpeed() const noexcept;
        float getMaxSpeed() const noexcept;

        void applyForce(Vector const &force) noexcept(noexcept(invMass > 0.f));

        void doPhysics(float time_) noexcept;

    private:
        float invMass;
        Vector velocity;
        Vector maxVelocity;
        Vector acceleration;
        Vector maxAcceleration;
    };

    class RigidBody : virtual public Object
    {
    public:
        explicit RigidBody(float const &mass) noexcept;
        ~RigidBody() noexcept;

        PhysicsComponent *getPhysics() const noexcept;

        void doPhysics(float time_) noexcept;

    protected:
        PhysicsComponent *physics;
    };

} // namespace tnt

#endif //! TNT_RIGID_BODY_HPP