#ifndef TNT_DOO_ECS_PHYSICS_HPP
#define TNT_DOO_ECS_PHYSICS_HPP

#include <execution>

#include "doo_ecs/Base.hpp"
#include "utils/Assert.hpp"

// TODO:
// non-movable bodies.
// springs.
// maximal speed/acceleration.
// move the bounding box when the body/object moves.
// modify the collision algorithm to return what kind of collision occurs (horizontal/vertical/none).
// (maybe):
// set some default values (ex for damping, mass) ??
// bring back physics_comp ??

namespace tnt::doo
{
    inline namespace phys
    {
        inline static constexpr float beta{0.05f};         /// < A constant used for baumgarte stabilization.
        inline static constexpr Vector gravity{0.f, 10.f}; /// < A vector acting as the gravity force.
    }                                                      // namespace phys

    enum class body_type
    {
        fixed = 0, /// < The body is fixed in a certain position.
        dynamic    /// < The body can move freely.
    };

    enum class collision_type
    {
        none = 0,   /// < The objects don't collide.
        horizontal, /// < The objects collide on their left/right side.
        vertical,   /// < The objects collide on their top/down side.
    };

    struct physics_comp final
    {
        body_type type;
        float mass;
        float damping;
        float restitution;
        Rectangle bound_box{};
    };

    /// @brief A struct that handles the physics data of all the objects.
    inline struct physics_sys final
    {
        /// @brief Add a new object to the physics system.
        /// @param body The physics component that has the data of the object.
        /// @note The position of the bounding box of the component is considered
        /// to be relative to the position of the body.
        inline void add_object(physics_comp const &body)
        {
            object const &id{inv_mass.size()};
            [[unlikely]] if (id == inv_mass.capacity())
            {
                inv_mass.reserve(10);

                damping.reserve(10);
                restitution.reserve(10);

                vel.reserve(10);
                accel.reserve(10);
                bound_box.reserve(10);
            }

            inv_mass.emplace_back(if_else(body.type == body_type::fixed, 0.f, 1 / body.mass));

            if (body.bound_box != Rectangle{0, 0, 0, 0})
                bound_box.emplace_back(body.bound_box + objects.pos[id]);
            else
                bound_box.emplace_back(
                    objects.pos[id],
                    objects.scale[id].x * sprites.clip[id].w,
                    objects.scale[id].y * sprites.clip[id].h);

            damping.emplace_back(body.damping);
            restitution.emplace_back(body.restitution);

            vel.emplace_back(VECTOR_ZERO);
            accel.emplace_back(VECTOR_ZERO);
        }

        /// @brief Apply the given force to the object with the given id.
        /// @param id The id of the object.
        /// @param force The force to apply to the object.
        inline void addForce(object id, tnt::Vector const &force) noexcept
        {
            accel[id] += (force * inv_mass[id]);
        }

        /// @brief Add a force that would be applied to each object when Update is called.
        /// @param force The force to be applied.
        inline void addGlobalForce(tnt::Vector const &force) noexcept
        {
            totalForce += force;
            std::for_each(
                std::execution::par_unseq,
                objects.active.cbegin(), objects.active.cend(),
                [this, &force](object const &id) noexcept -> void {
                    accel[id] += (totalForce * inv_mass[id]);
                });
        }

        /// @brief Update the physics data of the object with the given id.
        /// @param id The id of the object to update.
        /// @param time_ The time that passed since the last update call.
        inline void Update(object id, float time_) noexcept
        {
            check(time_ > 0.f, "Calling tnt::doo::physics_sys::Update with parameter time_ equal to 0. Objects will not be updated!!");
            float const &damp{std::powf(damping[id], time_)};

            objects.pos[id] += (physics.vel[id] * time_);
            bound_box[id] += (physics.vel[id] * time_);

            vel[id] += (accel[id] * time_);
            vel[id] *= damp;
        }

        /// @brief Check if the bounding boxes of the two objects are colliding.
        /// @param id The id of the first object.
        /// @param id2 The id of the second object.
        /// @return bool
        inline bool colliding(object id, object id2) noexcept
        {
            // TODO: check for collision when angle is non-zero.
            bool ret = !(bound_box[id2].x - (bound_box[id].x + bound_box[id].w) > 0.f ||
                         bound_box[id2].y - (bound_box[id].y + bound_box[id].h) > 0.f);
            ret = ret && !(bound_box[id].x - (bound_box[id2].x + bound_box[id2].w) > 0.f ||
                           bound_box[id].y - (bound_box[id2].y + bound_box[id2].h) > 0.f);

            return ret;
        }

        // thx Randy Gaul
        // https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331

        /// @brief Resolve the collision between the two objects.
        /// @param id The id of the first object.
        /// @param id2 The id of the second object.
        inline void resolve(object id, object id2) noexcept
        {
            Vector const &normal{[&id, &id2]() -> Vector {
                Vector const &n{objects.pos[id2] - objects.pos[id]};
                return n.Normalized();
            }()};

            Vector const &rel_vel{vel[id2] - vel[id]};
            float const &velAlongNormal{Dot(rel_vel, normal)};

            if (velAlongNormal > 0.f)
                return;

            float const &eps{if_else(restitution[id] < restitution[id2],
                                     restitution[id], restitution[id2])};
            float j{-(1 + eps) / velAlongNormal};
            j /= (inv_mass[id] + inv_mass[id2]);

            Vector const &impulse{normal * j};

            float const &totalMass{(1 / inv_mass[id]) + (1 / inv_mass[id2])};

            vel[id] -= impulse * totalMass * inv_mass[id];
            vel[id2] += impulse * totalMass * inv_mass[id2];
        }

        /// @brief Load objects physics data from a json chunk.
        /// @param j The json chunk that contains the objects data.
        inline void from_json(nlohmann::json const &j)
        {
            if (json_has(j, "phys"))
            {
                if (nlohmann::json const &phys{j["phys"]}; json_has(phys, "bounds"))
                    add_object(physics_comp{.mass{phys["mass"]}, .damping{phys["damping"]}, .restitution{phys["restitution"]}, .bound_box{phys["bounds"]}});
                else
                    add_object(physics_comp{.mass{phys["mass"]}, .damping{phys["damping"]}, .restitution{phys["restitution"]}});
            }
        }

        tnt::Vector totalForce{0.f, 0.f};

        std::vector<float> inv_mass; /// < The inverse of the mass of each objects.

        std::vector<float> damping;     /// < The damping of the objects.
        std::vector<float> restitution; /// < The restitution of the objects.

        std::vector<Vector> vel;   /// < The velocities of the objects.
        std::vector<Vector> accel; /// < The accelerations of the objects.

        std::vector<Rectangle> bound_box; /// < The bounding boxes of the bodies.
    } physics;                            /// < An instance of phys_sys.
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_PHYSICS_HPP