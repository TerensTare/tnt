#ifndef TNT_COMPONENT_HPP
#define TNT_COMPONENT_HPP

#include <string_view>
#include "math/Rectangle.hpp"

namespace tnt
{
    class Timer;

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
} // namespace tnt

#endif //! TNT_COMPONENT_HPP