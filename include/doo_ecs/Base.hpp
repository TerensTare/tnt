#ifndef TNT_DOO_ECS_BASE_HPP
#define TNT_DOO_ECS_BASE_HPP

#include <concepts>

// TODO:
// optimizations (ex. use pmr)
// particle_sys
// add constinit/constexpr
// spaces/scenes implemented as id-s only and operations on objects related to spaces/scenes (move, remove, etc).

// example of a simple update/draw loop with the new data oriented ECS
// someWindow.Clear();
// for (object const& o : objects.active)
// {
//      physics.Update(o, deltaTime);
//      animations.Update(o, deltaTime);
//
//      sprites.Draw(o, someWindow);
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
    using object = std::size_t;   /// < A data type that serves as a unique id of an object.
    using camera = std::uint32_t; /// < A data type that serves as a unique id of a camera.

    template <std::unsigned_integral I>
    inline constexpr I null_v{(I)-1};

    inline constexpr object null{null_v<object>}; /// < A special id having a context-dependent meaning.

    // template <typename T>
    // concept system = std::is_final_v<T> &&
    //     std::default_initializable<T> &&
    //         requires(T && t) {
    //             t.add_object;
    //             t.from_json;
    //             t.draw_imgui;
    //         } && (requires (T && t) { t.Update; }
    //          || requires (T && t) { t.Draw; });
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_BASE_HPP