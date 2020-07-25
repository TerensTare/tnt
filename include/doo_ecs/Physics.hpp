#ifndef TNT_DOO_ECS_PHYSICS_HPP
#define TNT_DOO_ECS_PHYSICS_HPP

#include <vector>
#include <nlohmann/json.hpp>

#include "math/Vector.hpp"
#include "doo_ecs/Base.hpp"

// (maybe) separate vectors for every data ??

namespace tnt::doo
{
    /// @brief A struct that handles an object's physics data.
    struct phys_comp final
    {
        /// @brief Create a new phys_comp.
        /// @param mass The mass of the new object.
        /// @param accel_ The initial acceleration of the object.
        inline constexpr phys_comp(float const &mass, tnt::Vector const &accel_) noexcept
            : inv_mass{1 / mass}, vel{0.f, 0.f}, accel{accel_} {}

        float inv_mass; /// < The mass of the object, but to power of -1.
        Vector vel;     /// < The velocity of the object.
        Vector accel;   /// < The acceleration of the object.
    };

    /// @brief A struct that handles the physics data of all the objects.
    inline struct physics_sys final
    {
        /// @brief Add a new object to the physics system.
        /// @param mass The mass of the object.
        inline void add_object(float const &mass)
        {
            [[unlikely]] if (inv_mass.size() == inv_mass.capacity())
            {
                inv_mass.reserve(5);
                vel.reserve(5);
                accel.reserve(5);
            }

            inv_mass.emplace_back(1 / mass);
            vel.emplace_back(VECTOR_ZERO);
            accel.emplace_back(VECTOR_ZERO);
        }

        /// @brief Apply the given force to the object with the given id.
        /// @param id The id of the object.
        /// @param force The force to apply to the object.
        inline void addForce(object id, tnt::Vector const &force) noexcept
        {
            accel[id] = accel[id] + (force * inv_mass[id]);
        }

        /// @brief Update the physics data of the object with the given id.
        /// @param id The id of the object to update.
        /// @param time_ The time that passed since the last update call.
        inline void Update(object id, float time_) noexcept
        {
            vel[id] = vel[id] + (accel[id] * float(time_ / 1000));
        }

        /// @brief Load objects physics data from a json chunk.
        /// @param j The json chunk that contains the objects data.
        inline void from_json(nlohmann::json const &j)
        {
            add_object(j.at("phys").at("mass"));
        }

        std::vector<float> inv_mass; /// < The inverse of the mass of each objects.
        std::vector<Vector> vel;     /// < The velocities of the objects.
        std::vector<Vector> accel;   /// < The accelerations of the objects.
    } physics;                       /// < An instance of phys_sys.
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_PHYSICS_HPP