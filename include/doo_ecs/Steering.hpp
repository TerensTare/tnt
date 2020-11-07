#ifndef TNT_DOO_ECS_STEERING_BEHAVIOURS_HPP
#define TNT_DOO_ECS_STEERING_BEHAVIOURS_HPP

#include "doo_ecs/Base.hpp"

namespace tnt::doo
{
    /// @brief A system that adds support steering behaviours to the objects.
    inline struct steering_sys final
    {
        steering_sys() = default;

        // steering_sys(steering_sys const &) = delete;
        // steering_sys &operator=(steering_sys const &) = delete;

        /// @brief Move object @a id towards @a target.
        /// @param id The id of the moving object.
        /// @param target The desired position in global coordinates.
        /// @note Use @ref Pursuit if @a target is supposed to be moving. It gives more "natural" results.
        /// @sa @ref Pursuit
        void Seek(object const &id, Vector const &target) const noexcept;

        /// @brief Move object @a id away from @a target.
        /// @param id The id of the moving object.
        /// @param target The position that the object will avoid.
        /// @note Use @ref RangedFlee if you want @a obj to stay at a certain distance away from target.
        /// @note Use @ref Evade if @a target is supposed to be moving. It gives more "natural" results.
        /// @sa @ref RangedFlee
        /// @sa @ref Evade
        void Flee(object const &id, Vector const &target) const noexcept;

        /// @brief Move object @a id away from @a target if it is close enough.
        /// @param id The id of the moving object.
        /// @param target The center of the circular area to be avoided.
        /// @param range The minimum distance that the object and the target can have.
        /// @sa @ref Flee
        /// @sa @ref Evade
        void RangedFlee(object const &id, Vector const &target, float range) const noexcept;

        /// @brief Direct the given object towards @a target with a speed that decreases as distance decreases.
        /// @param id The object to make seek towards @a target.
        /// @param target The position where @a obj will be directed to.
        void Arrive(object const &id, Vector const &target) const noexcept;

        /// @brief Direct the first object towards @a target. Useful when @a target is supposed to be moving.
        /// @param id The object to make seek towards @a target.
        /// @param target The object which @a obj will follow.
        /// @note Use @ref Seek if @a target is not supposed to be moving.
        /// @sa @ref Seek
        void Pursuit(object const &id, object const &target) const noexcept;

        /// @brief Move object @a id away from @a target. Useful when @a target is supposed to be moving.
        /// @param id The object to flee away from @a target.
        /// @param target The object which @a obj will avoid.
        /// @note Use @ref Flee if @a target is not supposed to be moving.
        /// @sa @ref tnt::steer::Flee
        void Evade(object const &id, object const &target) const noexcept;
    } steer; /// < An instance of steering_sys.
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_STEERING_BEHAVIOURS_HPP