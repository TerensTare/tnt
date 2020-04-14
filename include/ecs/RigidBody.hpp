#ifndef TNT_RIGID_BODY_HPP
#define TNT_RIGID_BODY_HPP

#include "ecs/Component.hpp"
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
    // TODO: collision_box is not used. Find a way to use it.
    class RigidBody : virtual public Object
    {
      public:
        RigidBody(float mass, Rectangle const &collision_box);
        ~RigidBody() noexcept;

        PhysicsComponent *getPhysics() const noexcept; // (maybe) not const

      protected:
        PhysicsComponent *physics;
    };

    class Joint
    {
      protected: // should this be private ??
        RigidBody *poles[2];
    };
} // namespace tnt

#endif //! TNT_RIGID_BODY_HPP