#ifndef TNT_DOO_ECS_2D_SKELETAL_ANIMATIONS_SYSTEM_HPP
#define TNT_DOO_ECS_2D_SKELETAL_ANIMATIONS_SYSTEM_HPP

#include <nlohmann/json_fwd.hpp>
#include "doo_ecs/Base.hpp"

// a basic json file:
// [
//   {
//     // ommited stuff from other systems
//     "bones": [
//       {
//         "head": {
//           "x": 10,
//           "y": 10
//         },
//         "tail": {
//           "x": 100,
//           "y": 100
//         },
//         "file": "head.png"
//       }
//     ]
//   }
// ]

// TODO(maybe):
// consider the bottom as the position of the bone.

namespace tnt::doo
{
    /// @brief A basic struct representing a bone used for skeletal animation.
    struct bone_data final
    {
        float length;          /// < The length of the bone.
        float minAngle{0.f};   /// < The minimum angle the bone can have.
        float maxAngle{360.f}; /// < The maximum angle the bone can have.
    };

    /// @brief A basic struct representing a sequence of bones connected to each-other.
    struct bone_tree final
    {
        /// @brief Create a new bone tree with a single bone on it.
        /// @param data_ The data of the first bone of the tree.
        bone_tree(bone_data const &data_);

        /// @brief Attach a new bone to an existing one.
        /// @param parent_ The bone serving as a parent to the new one.
        /// @param data_ The data of the new bone.
        void add_bone(object const &parent_, bone_data const &data_);

        std::vector<float> length;   /// < The lengths of the bones.
        std::vector<float> minAngle; /// < The minimum angle that each bone can form.
        std::vector<float> maxAngle; /// < The maximum angle that each bone can form.

        std::vector<object> ids;     /// < The id-s of the bones.
        std::vector<object> parents; /// < The id-s of the parent of each bone.
    };

    /// @brief A system that handles bones animations.
    inline struct bones_sys final
    {
        bones_sys() = default;

        bones_sys(bones_sys const &) = delete;
        bones_sys &operator=(bones_sys const &) = delete;

        /// @brief Add a new object to the bones system.
        /// @param data_ The data that the object should contain.
        void add_object(bone_data const &data_);

        /// @brief Add a new invalid object to the bones system.
        void add_invalid();

        /// @brief Add new objects to the bones system by using data from a json chunk.
        /// @param j The json chunk containing the objects data.
        void from_json(nlohmann::json const &j);

        std::vector<object> active;   /// < The id-s of the active bone trees. Not related to the id-s of the other objects.
        std::vector<bone_tree> trees; /// < The datas of the bone trees.
    } bones;                          /// < An instance of bones_sys
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_2D_SKELETAL_ANIMATIONS_SYSTEM_HPP