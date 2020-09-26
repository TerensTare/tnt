#ifndef TNT_DOO_ECS_BASE_HPP
#define TNT_DOO_ECS_BASE_HPP

#include <span>
#include <nlohmann/json.hpp>

#include "math/Vector.hpp"
#include "core/Window.hpp"

// TODO:
// optimizations (ex. use pmr)
// ensure objects index on each system and each vector is the same.
// load to lua (needs tests)
// particle_sys
// add constinit/constexpr
// spaces/scenes implemented as id-s only and operations on objects related to spaces/scenes (move, remove, etc).
// TODO(maybe):
// add_object ONLY on objects_sys, and pass an object_data const& (a base class) to it ??

// example of a simple update/draw loop with the new data oriented ECS
// someWindow.Clear();
// for (object const& o : objects.active)
// {
//      physics.Update(o, deltaTime);
//      animations.Update(o, deltaTime);
//      if (tnt::doo::object const s{sprites.draw_queue.size()};
//          s > obj && sprites.draw_queue[obj] != -1)
//          sprites.Draw(o, someWindow);
// }
// someWindow.Render();

// example of a .json chunk containing objects data
// [
//     {
//         "angle": 0,
//         "scale": {
//             "x": 10,
//             "y": 10
//         },
//         "pos": {
//             "x": 100,
//             "y": 100
//         },
//         "phys": {
//             "mass": 1,
//             "bounds": {
//                 "x": 0,
//                 "y": 100,
//                 "w": 100,
//                 "h": 60
//             }
//         },
//         "sprite": {
//             "file": "assets/player.png",
//             "crop": {
//                 "x": 3,
//                 "y": 0,
//                 "w": 10,
//                 "h": 16
//             }
//         },
//         "anim": {
//             "frames": 7,
//             "space": 6,
//             "speed": 1400,
//             "dir": "horizontal",
//             "wrap": "loop"
//         }
//     }
// ...
// ]
// NOTE: "phys.bounds" can be omitted to use the whole image area as the bounding box.
// NOTE: "sprite.crop" can be omitted to get the whole image to the texture.

namespace tnt::doo
{
    using object = std::size_t; /// < A data type that serves as a unique id of an object.

    /// @brief Check if the given object exists on the given system's object list.
    /// @param members The list of the id-s of the members of a system.
    /// @param id The id of the desired object.
    /// @return bool
    inline const auto has_object = [](std::span<object> members, object const &id) noexcept -> bool {
        return (members.size() > id && members[id] != -1);
    };

    /// @brief Check if the given json chunk has a certain field.
    /// @param j The json chunk to check.
    /// @param key The desired key.
    /// @return bool
    inline const auto json_has = [](nlohmann::json const &j, char const *key) noexcept(
                                     noexcept(j.find(key))) -> bool {
        return (j.find(key) != j.cend());
    };

    /// @brief A struct that holds the basic data of an object.
    struct object_data final
    {
        /// @brief Create a new object data handle from the given params.
        /// @param angle_ The initial angle of the object.
        /// @param scale_ The initial scale of the object.
        /// @param pos_ The initial position of the object.
        constexpr object_data(float angle_, Vector const &pos_, Vector const &scale_,
                              object const &parent_ = (object)-1) noexcept;

        float angle;   /// < The angle of the object.
        Vector scale;  /// < The scale of the object.
        Vector pos;    /// < The position of the object.
        object parent; /// < The parent of the object.
    };

    /// @brief A struct that handles objects data.
    inline struct objects_sys final
    {
        objects_sys() = default;

        objects_sys(objects_sys const &) = delete;
        objects_sys &operator=(objects_sys const &) = delete;

        /// @brief Create a new object in place and add it to the system.
        /// Return the id of the newly created @c object.
        /// @return tnt::doo::object
        object add_object(object_data const &data_) noexcept;

        /// @brief Get the data of the object with the given id.
        /// @param id The id of the object.
        /// @return object_data
        [[nodiscard]] object_data get_data(object const &id) const noexcept;

        /// @brief Load objects data from a json chunk.
        /// @param j The json chunk that contains the objects data.
        void from_json(nlohmann::json const &j);

        /// @brief Draw widgets on the given window to modify the datas of the system.
        /// @param id The id of the active object.
        /// @param win The window where to draw the widgets.
        void draw_imgui(object const &id, Window const &win) noexcept;

        /// @brief Get the angle of the object in global context.
        /// @param id The id of the object.
        /// @return float
        /// @note The function returns angle[id] if the object has no parent (aka. parent[id] == -1).
        [[nodiscard]] float gAngle(object const &id) const noexcept;

        /// @brief Get the scale of the object in global context.
        /// @param id The id of the object.
        /// @return Vector
        /// @note The function returns scale[id] if the object has no parent (aka. parent[id] == -1).
        [[nodiscard]] Vector gScale(object const &id) const noexcept;

        /// @brief Get the position of the object in global context.
        /// @param id The id of the object.
        /// @return Vector
        /// @note The function returns pos[id] if the object has no parent (aka. parent[id] == -1).
        [[nodiscard]] Vector gPos(object const &id) const noexcept;

        /// @brief Change the parent of the given object and apply all the necessary changes.
        /// @param id The id of the child object.
        /// @param parent_ The id of the new parent.
        void set_parent(object const &id, object const &parent_) noexcept;

        std::vector<float> angle; /// < The angles of the objects.

        std::vector<object> active; /// < The id-s of all the active objects.
        std::vector<object> parent; /// < The id-s of the parents of the objects.

        std::vector<Vector> scale; /// < The scales of the objects.
        std::vector<Vector> pos;   /// < The positions of the objects.
    } objects;                     /// < An instance of objects_sys.

    // clang-format off
    template <typename T>
    concept system = std::is_final_v<T> && std::same_as<T, objects_sys> ||
        (std::default_initializable<T> && !std::copy_constructible<T> &&
            requires(T && t) {
                t.add_object;
                t.add_invalid;
                t.from_json;
                t.draw_imgui;
            } && (requires (T && t) { t.Update; }
             || requires (T && t) { t.Draw; }));
    // clang-format on
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_BASE_HPP