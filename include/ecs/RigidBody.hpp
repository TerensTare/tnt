#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include "Object.hpp"

// TODO:
// find a way to apply forces to the RigidBody.
// handle collision in Update.
// TODO(maybe):
// Update() from outside RigidBody ??
// use a force accumulator for adding forces ??

namespace tnt
{

class RigidBody : public Object
{
public:
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