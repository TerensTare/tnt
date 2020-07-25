#ifndef TNT_DOO_ECS_OBJECTS_HPP
#define TNT_DOO_ECS_OBJECTS_HPP

#include "utils/Assert.hpp"

#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Sprites.hpp"

#include "json/JsonDooObject.hpp"

// TODO:
// optimizations (ex. use pmr)
// ensure objects index on each system and each vector is the same.
// check for non-existent properties when loading from json.
// load to lua
// particle_sys
// TODO(maybe):
// remove the adl_serializer<object_data> ??
// separate the objects data from the objects Update()/Draw() ??
// or (maybe) store the scale on the sprite_sys ??
// add_object ONLY on objects_sys, and pass an object_params const& (a base class) to it.

// example of a simple loop with the new data oriented ECS
// someWindow.Clear();
// for (object const& o : objects.active)
// {
//      physics.Update(o, deltaTime);
//      objects.Update(o, deltaTime);
//      if (sprites.draw_queue.contains(obj))
//          objects.Draw(o, someWindow);
// }
// someWindow.Render();

// example of a .json file to load all objects
// [
//     {
//         "angle": 0,
//         "scale": {
//             "x": 1,
//             "y": 1
//         },
//         "pos": {
//             "x": 100,
//             "y": 100
//         },
//         "phys": {
//             "mass": 1
//         },
//         "sprite": {
//             "file": "myFile.png",
//             "crop": {
//                 "x": 0,
//                 "y": 0,
//                 "w": 8,
//                 "h": 8
//             }
//         }
//     },
//     ...
// ]
// NOTE: crop can be null to get the whole image to the texture.

namespace tnt::doo
{
    /// @brief A struct that handles objects data.
    inline struct objects_sys final
    {
        /// @brief Create a new object in place and add it to the system.
        /// Return the id of the newly created @c object.
        /// @return tnt::doo::object
        inline object add_object(object_data const &data_) noexcept
        {
            const object index{angle.size()};

            [[unlikely]] if (index == angle.capacity()) // if the vector got to his capacity, then we reserve more space
            {
                active.reserve(5);
                angle.reserve(5);
                scale.reserve(5);
                pos.reserve(5);
            }

            active.emplace_back(index);
            passive.emplace_back(-1);
            angle.emplace_back(data_.angle);
            scale.emplace_back(data_.scale);
            pos.emplace_back(data_.pos);

            return index;
        }

        /// @brief Update the object with the given id.
        /// @param id The id of the object to update.
        /// @param time_ The time elapsed since the last Update() call.
        inline void Update(object id, float time_) noexcept
        {
            check(time_ > 0.f, "Calling tnt::doo::objects_sys::Update with parameter time_ begin 0. Objects will not be updated");
            pos[id] += (physics.vel[id] * float(time_ / 1000));
        }

        /// @brief Draw object with the given id on the given window.
        /// @param id The id of the object to draw.
        /// @param win The window where the object will be drawed.
        inline void Draw(object id, Window const &win) noexcept
        {
            float const dx{sprites.clip[id].w * scale[id].x * .5f};
            float const dy{sprites.clip[id].h * scale[id].y * .5f};
            SDL_FRect const dst{pos[id].x - dx, pos[id].y - dy, 2 * dx, 2 * dy};

            SDL_RenderCopyExF(
                win.getRenderer(), sprites.tex[id],
                &sprites.clip[id], &dst, angle[id],
                nullptr, SDL_FLIP_NONE);
        }

        /// @brief Get the data of the object with the given id.
        /// @param id The id of the object.
        /// @return object_data
        inline object_data get_data(object id) const noexcept
        {
            return object_data{angle[id], scale[id], pos[id]};
        }

        /// @brief Load objects data from a json chunk.
        /// @param j The json chunk that contains the objects data.
        inline void from_json(nlohmann::json const &j)
        {
            add_object(j.get<object_data>());
        }

        std::vector<object> active; /// < The id-s of all the active objects.

        std::vector<object> passive; /// < The id-s of all the non-active objects.

        std::vector<float> angle;  /// < The angles of the objects.
        std::vector<Vector> scale; /// < The scales of the objects.
        std::vector<Vector> pos;   /// < The positions of the objects.
    } objects;                     /// < An instance of objects_sys.
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_OBJECTS_HPP