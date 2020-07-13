#ifndef TNT_DOO_ECS_PHYSICS_HPP
#define TNT_DOO_ECS_PHYSICS_HPP

#include <vector>
#include <nlohmann/json.hpp>

#include "math/Vector.hpp"
#include "doo_ecs/Base.hpp"

namespace tnt::doo
{
    struct phys_comp final
    {
        inline constexpr phys_comp(float const &mass, tnt::Vector const &accel_) noexcept
            : inv_mass{1 / mass}, vel{0.f, 0.f}, accel{accel_} {}

        float inv_mass;
        Vector vel;
        Vector accel;
    };

    class physics_sys final
    {
    public:
        inline void add_object(float const &mass)
        {
            [[unlikely]] if (phys.size() == phys.capacity())
                phys.reserve(5);
            phys.emplace_back(1 / mass, tnt::Vector{0.f, 0.f});
        }

        inline void addForce(object id, tnt::Vector const &force) noexcept
        {
            phys[id].accel = phys[id].accel + (force * phys[id].inv_mass);
        }

        inline void Update(object id, long long time_) noexcept
        {
            phys[id].vel = phys[id].vel + (phys[id].accel * float(time_ / 1000));
        }

        inline void from_json(nlohmann::json const &j)
        {
            float const mass{j.at("phys").at("mass")};
            add_object(mass);
        }

        std::vector<phys_comp> phys;
    } physics;
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_PHYSICS_HPP