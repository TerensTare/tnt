#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include <exception>
#include "Object.hpp"

// TODO:
// find a way to apply forces to the RigidBody.
// handle collision in Update.
// TODO(maybe):
// Update() from outside RigidBody ??
// use a force accumulator for adding forces ??

namespace tnt
{
struct infinite_mass : std::exception
{
    const char *what() const noexcept
    {
        return "RigidBody has infinite mass!!";
    }
};

class RigidBody : public Object
{
public:
    void setMass(float mass);
    float getMass() const noexcept(noexcept(invMass > 0.f));

    Vector getVelocity() const noexcept;
    Vector getAcceleration() const noexcept;

    void applyForce(Vector const &force) noexcept(noexcept(invMass > 0.f));

    virtual void Update() override;

protected:
    float invMass;
    Vector velocity;
    Vector acceleration;
};

class Joint : public RigidBody
{
};
} // namespace tnt

#endif //!RIGID_BODY_HPP