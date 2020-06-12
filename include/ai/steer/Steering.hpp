#ifndef TNT_AI_STEERING_HPP
#define TNT_AI_STEERING_HPP

#include "math/Vector.hpp"

// utility file containing steering behaviours support for tnt::object.
// NOTE: the object should have PhysicsComponent and these functions should be used like:
// MyPhysicsObjectType myObject;
// myObject.applyForce(tnt::steer::SteeringFunction(obj, ..));

namespace tnt
{
    class Object;
}

namespace tnt::steer
{
    /// @brief Direct @c obj towards @c target.
    /// @param obj The object to make seek towards @c target.
    /// @param target The position where @c obj will be directed to.
    /// @return tnt::Vector
    /// @note Use Pursuit if @c target is supposed to be moving. It gives more "natural" results.
    /// @sa tnt::steer::Pursuit
    Vector Seek(Object const *obj, Vector const &target);

    /// @brief Move @c obj away of @c target.
    /// @param obj The object to make flee from @c target.
    /// @param target The position that @c obj will try to avoid.
    /// @return tnt::Vector
    /// @note Use RangedFlee if you want @c obj to stay at a certain distance away from target.
    /// @note Use Evade if @c target is supposed to be moving. It gives more "natural" results.
    /// @sa tnt::steer::RangedFlee
    /// @sa tnt::steer::Evade
    Vector Flee(Object const *obj, Vector const &target);

    /// @brief Move @c obj away of @c target if their distance is less than @c range.
    /// @param obj The object to make flee from @c target.
    /// @param target The position that @c obj will try to avoid.
    /// @param range The minimum distance that @c obj and @c target are allowed to have.
    /// @return tnt::Vector
    /// @sa tnt::steer::Flee
    /// @sa tnt::steer::Evade
    Vector RangedFlee(Object const *obj, Vector const &target, float range);

    /// @brief Direct @c obj towards @c target with a speed that decreases as distance decreases.
    /// @param obj The object to make seek towards @c target.
    /// @param target The position where @c obj will be directed to.
    /// @param decel The deceleration of @c obj as he approaches to @c target.
    /// @return tnt::Vector
    /// @note @c obj's deceleration will be slow if @c decel is 3,
    /// fast if @c decel is 1, or normal if @c decel is 2 (default).
    Vector Arrive(Object const *obj, Vector const &target, int decel = 2);

    /// @brief Direct @c obj towards @c target. Useful when @c target is supposed to be moving.
    /// @param obj The object to make seek towards @c target.
    /// @param target The object which @c obj will follow.
    /// @return tnt::Vector
    /// @note Use Seek if @c target is not supposed to be moving.
    /// @sa tnt::steer::Seek
    Vector Pursuit(Object const *obj, Object const *target);

    /// @brief Move @c obj away of @c target.
    /// @param obj The object to make flee from @c target.
    /// @param target The position that @c obj will try to avoid.
    /// @return tnt::Vector
    /// @note Use RangedFlee if you want @c obj to stay at a certain distance away from target.
    /// @note Use Evade if @c target is supposed to be moving. It gives more "natural" results.
    /// @sa tnt::steer::RangedFlee
    /// @sa tnt::steer::Evade
    Vector Evade(Object const *obj, Object const *target);
} // namespace tnt::steer

#endif //!TNT_AI_STEERING_HPP