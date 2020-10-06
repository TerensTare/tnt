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
    using camera = std::size_t; /// < A data type that serves as a unique id of a camera.

    inline constexpr object null{(object)-1}; /// < An object id indicating that the object is abset on the desired system.

    /// @brief Check if the given object exists on the given system's object list.
    /// @param members The list of the id-s of the members of a system.
    /// @param id The id of the desired object.
    /// @return bool
    inline const auto has_object = [](std::span<object> members, object const &id) noexcept -> bool {
        return (members.size() > id && members[id] != null);
    };

    /// @brief Check if the given json chunk has a certain field.
    /// @param j The json chunk to check.
    /// @param key The desired key.
    /// @return bool
    inline const auto json_has = [](nlohmann::json const &j, char const *key) noexcept(
                                     noexcept(j.find(key))) -> bool {
        return (j.find(key) != j.cend());
    };

    // clang-format off
    template <typename T>
    concept basic_system = std::is_final_v<T> &&
        std::default_initializable<T> && 
        !std::copy_constructible<T> &&
            requires(T && t) {
                t.add_object;
                t.add_invalid;
                t.from_json;
                t.draw_imgui;
            } && (requires (T && t) { t.Update; }
             || requires (T && t) { t.Draw; });
    // clang-format on
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_BASE_HPP