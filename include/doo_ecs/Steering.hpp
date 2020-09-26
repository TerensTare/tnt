#ifndef TNT_DOO_ECS_STEERING_BEHAVIOURS_HPP
#define TNT_DOO_ECS_STEERING_BEHAVIOURS_HPP

#include "doo_ecs/Base.hpp"

namespace tnt::doo
{
    /// @brief A system that adds support steering behaviours to the objects.
    inline struct steering_sys final
    {
        steering_sys() = default;

        steering_sys(steering_sys const &) = delete;
        steering_sys &operator=(steering_sys const &) = delete;

        /// @brief Move object @c id towards @c target.
        /// @param id The id of the moving object.
        /// @param target The desired position in global coordinates.
        /// @note Use Pursuit if @c target is supposed to be moving. It gives more "natural" results.
        /// @sa tnt::doo::steering_sys::Pursuit
        void Seek(object const &id, Vector const &target) const noexcept;

        /// @brief Move object @c id away from @c target.
        /// @param id The id of the moving object.
        /// @param target The position that the object will avoid.
        /// @note Use RangedFlee if you want @c obj to stay at a certain distance away from target.
        /// @note Use Evade if @c target is supposed to be moving. It gives more "natural" results.
        /// @sa tnt::doo::steering_sys::RangedFlee
        /// @sa tnt::doo::steering_sys::Evade
        void Flee(object const &id, Vector const &target) const noexcept;

        /// @brief Move object @c id away from @c target if it is close enough.
        /// @param id The id of the moving object.
        /// @param target The center of the circular area to be avoided.
        /// @param range The minimum distance that the object and the target can have.
        /// @sa tnt::doo::steering_sys::Flee
        /// @sa tnt::doo::steering_sys::Evade
        void RangedFlee(object const &id, Vector const &target, float range) const noexcept;

        /// @brief Direct the given object towards @c target with a speed that decreases as distance decreases.
        /// @param obj The object to make seek towards @c target.
        /// @param target The position where @c obj will be directed to.
        void Arrive(object const &id, Vector const &target) const noexcept;

        /// @brief Direct the first object towards @c target. Useful when @c target is supposed to be moving.
        /// @param id The object to make seek towards @c target.
        /// @param target The object which @c obj will follow.
        /// @note Use Seek if @c target is not supposed to be moving.
        /// @sa tnt::steer::Seek
        void Pursuit(object const &id, object const &target) const noexcept;

        /// @brief Move object @c id away from @c target. Useful when @c target is supposed to be moving.
        /// @param id The object to flee away from @c target.
        /// @param target The object which @c obj will avoid.
        /// @note Use Flee if @c target is not supposed to be moving.
        /// @sa tnt::steer::Flee
        void Evade(object const &id, object const &target) const noexcept;
    } steer; /// < An instance of steering_sys.
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_STEERING_BEHAVIOURS_HPP