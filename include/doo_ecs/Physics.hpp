#ifndef TNT_DOO_ECS_PHYSICS_HPP
#define TNT_DOO_ECS_PHYSICS_HPP

#include "doo_ecs/Base.hpp"
#include "math/Rectangle.hpp"
#include "utils/SparseSet.hpp"

// TODO:
// handle cases when the object's parent changes (so does it's speed, etc).
// springs.
// when adding a new fixed object, ignore it's mass value.
// modify the collision algorithm to return what kind of collision occurs (horizontal/vertical/none).
// custom collision polygons.

// (maybe):
// set some default values (ex for damping, mass) ??

namespace tnt::doo
{
    inline namespace phys
    {
        inline constexpr float beta{0.05f};         /// < A constant used for baumgarte stabilization.
        inline constexpr Vector gravity{0.f, 10.f}; /// < A vector acting as the gravity force.
    }                                               // namespace phys

    enum class body_type : uint8_t
    {
        fixed = 0, /// < The body is fixed in a certain position.
        kinematic, /// < The body can move freely and collide only with kinematic bodies.
        dynamic    /// < The body can move freely and collide with dynamic or fixed bodies.
    };

    enum class collision_type
    {
        none = 0,   /// < The objects don't collide.
        horizontal, /// < The objects collide on their left/right side.
        vertical,   /// < The objects collide on their top/down side.
    };

    struct physics_comp final
    {
        body_type type{body_type::dynamic};
        float mass;
        float damping;
        float restitution;
        Vector maxVel{40.f, 40.f};
        Vector maxAccel{15.f, 15.f};
        Rectangle bound_box;
    };

    /// @brief A struct that handles the physics data of all the objects.
    inline struct physics_sys final
    {
        physics_sys() = default;

        // physics_sys(physics_sys const &) = delete;
        // physics_sys &operator=(physics_sys const &) = delete;

        /// @brief Add a new object to the physics system.
        /// @param id The id of the object to add to the physics system.
        /// @param body The physics component that has the data of the object.
        /// @note The position of the bounding box of the component is considered
        /// to be relative to the position of the body.
        void add_object(object const &id, physics_comp const &body);

        /// @brief Apply the given force to the object with the given id.
        /// @param id The id of the object.
        /// @param force The force to apply to the object.
        void addForce(object const &id, tnt::Vector const &force) noexcept;

        /// @brief Add a force that would be applied to each object when Update is called.
        /// @param force The force to be applied.
        void addGlobalForce(tnt::Vector const &force) noexcept;

        /// @brief Update the physics data of the object with the given id.
        /// @param id The id of the object to update.
        /// @param time_ The time that passed since the last update call.
        void Update(object const &id, float time_) noexcept;

        /// @brief Check if the bounding boxes of the two objects are colliding.
        /// @param id The id of the first object.
        /// @param id2 The id of the second object.
        /// @return bool
        bool colliding(object const &id, object const &id2) noexcept;

        /// @brief Resolve the velocity between two objects that have collided.
        /// @param id The id of the first object.
        /// @param id2 The id of the second object.
        /// @note This is called by resolve().
        void resolveVel(object const &id, object const &id2) noexcept;

        /// @brief Resolve the interpenetration between two objects that have collided.
        /// @param id The id of the first object.
        /// @param id2 The id of the second object.
        /// @note This is called by resolve().
        void resolveInterpenetration(object const &id, object const &id2) noexcept;

        /// @brief Resolve the collision between two objects.
        /// @param id The id of the first object.
        /// @param id2 The id of the second object.
        void resolve(object const &id, object const &id2) noexcept;

        /// @brief Load objects physics data from a json chunk.
        /// @param id The id of the object to load from json.
        /// @param j The json chunk that contains the objects data.
        void from_json(object const &id, nlohmann::json const &j);

        /// @brief Draw widgets on the given window to modify the datas of the system.
        /// @param id The id of the active object.
        /// @param win The window where to draw the widgets.
        void draw_imgui(object const &id, Window const &win) noexcept;

        /// @brief Get the velocity of the object in global context.
        /// @param id The id of the object.
        [[nodiscard]] Vector gVel(object const &id) const noexcept;

        /// @brief Get the maximal velocity of the object in global context.
        /// @param id The id of the object.
        [[nodiscard]] Vector gMaxVel(object const &id) const noexcept;

        /// @brief Get the acceleration of the object in global context.
        /// @param id The id of the object.
        [[nodiscard]] Vector gAccel(object const &id) const noexcept;

        /// @brief Get the maximum acceleration of the object in global context.
        /// @param id The id of the object.
        [[nodiscard]] Vector gMaxAccel(object const &id) const noexcept;

        tnt::Vector totalForce{0.f, 0.f};

        std::vector<float> inv_mass; /// < The inverse of the mass of each objects.

        std::vector<float> damping;     /// < The damping of the objects.
        std::vector<float> restitution; /// < The restitution of the objects.

        std::vector<Vector> vel;          /// < The velocities of the objects.
        std::vector<Vector> maxVel;       /// < The maximal velocities of the objects.
        std::vector<Vector> accel;        /// < The accelerations of the objects.
        std::vector<Vector> maxAccel;     /// < The maximal accelerations of the objects.
        std::vector<Rectangle> bound_box; /// < The bounding boxes of the bodies.

        sparse_set<object> active; /// < The id-s of all the objects of the physics system.
    } physics;                     /// < An instance of phys_sys.
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_PHYSICS_HPP