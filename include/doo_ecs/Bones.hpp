#ifndef TNT_DOO_ECS_2D_SKELETAL_ANIMATIONS_SYSTEM_HPP
#define TNT_DOO_ECS_2D_SKELETAL_ANIMATIONS_SYSTEM_HPP

#include <nlohmann/json_fwd.hpp>

#include "doo_ecs/Base.hpp"
#include "math/Vector.hpp"
#include "utils/SparseSet.hpp"

// a basic json file:
// [
//   {
//     // ommited stuff from other systems
//     "bone": {
//         "length":10,
//         "joint": {
//             "x": 100,
//             "y": 100
//     }
// }
//   }
// ]

// TODO: modify sprites_sys::Draw to render using joint as point of rotation.

namespace tnt::doo
{
    /// @brief A basic struct representing a bone used for skeletal animation.
    struct bone_comp final
    {
        Vector joint; /// < One end of the bone.
        float length; /// < The length of the bone.
    };

    /// @brief A system that handles bones animations.
    inline struct bones_sys final
    {
        bones_sys() = default;

        /// @brief Add a new object to the bones system.
        /// @param id The id of the object that will have a bone component.
        /// @param data_ The data that the object will contain.
        void add_object(object const &id, bone_comp const &data_);

        /// @brief Add new objects to the bones system by using data from a json chunk.
        /// @param id The id of the object to add to the bones system.
        /// @param j The json chunk containing the objects data.
        void from_json(object const &id, nlohmann::json const &j);

        /// @brief Remove an object from the bones system.
        /// @param id The id of the object to remove.
        void remove(object const &id) noexcept;

        /// @brief Remove all the objects from the bones system.
        void clear() noexcept;

        /// @brief Get the other end of the bone of the given object.
        /// @param id The id of the object that has the desired bone.
        /// @return @ref tnt::Vector
        Vector bottom(object const &id) const noexcept;

        /// @brief Move the desired joint towards @a target.
        /// @param id The id of the joint you want to reach the target.
        /// @param target The destination you want the joint to reach.
        void follow(object const &id, Vector const &target) noexcept;

        std::vector<float> length; /// < The length of each bone.
        std::vector<Vector> joint; /// < The datas of the bone trees.
        sparse_set<object> active; /// < The id-s of the active bone trees. Not related to the id-s of the other objects.
    } bones;                       /// < An instance of bones_sys
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_2D_SKELETAL_ANIMATIONS_SYSTEM_HPP