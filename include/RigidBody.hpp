#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include <exception>
#include "Object.hpp"

// TODO:
// find a way to apply forces to the RigidBody.
// TODO(maybe):
// Update() from outside RigidBody ??

namespace tnt
{
struct infinite_mass : std::exception
{
    const char *what() const noexcept
    {
        return "body has infinite mass!!";
    }
};

class RigidBody : public Object
{
public:
    void setMass(float mass) noexcept;
    float getMass() const noexcept(noexcept(invMass > 0.f));

    Vector getVelocity() const noexcept;
    Vector getAcceleration() const noexcept;

protected:
    float invMass;
    Vector velocity;
    Vector acceleration;
};

class Joint : public Object
{
};
} // namespace tnt

#endif //!RIGID_BODY_HPP