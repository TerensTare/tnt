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
        PhysicsComponent(float const &mass, Rectangle const &collision_box);
        PhysicsComponent(float const &mass, float x, float y, float &w, float &h);

        void setMass(float const &mass);
        float getMass() const noexcept(noexcept(invMass > 0.f));

        Vector getVelocity() const noexcept;
        Vector getAcceleration() const noexcept;

        Rectangle getCollisionBox() const noexcept;

        void applyForce(Vector const &force) noexcept(noexcept(invMass > 0.f));

    private:
        float invMass;
        Vector velocity;
        Vector maxVelocity; // necessary ??
        Vector acceleration;
        Rectangle collisionBox;
    };

    // TODO: collision_box is not used. Find a way to use it.
    class RigidBody : virtual public Object
    {
    public:
        RigidBody(float const &mass, Rectangle const &collision_box);
        ~RigidBody() noexcept;

        PhysicsComponent *getPhysics() const noexcept;

    protected:
        PhysicsComponent *physics;
    };

} // namespace tnt

#endif //! TNT_RIGID_BODY_HPP