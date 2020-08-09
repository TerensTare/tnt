#ifndef TNT_DOO_ECS_BASE_HPP
#define TNT_DOO_ECS_BASE_HPP

#include <nlohmann/json.hpp>
#include "json/JsonVector.hpp"

// TODO:
// optimizations (ex. use pmr)
// ensure objects index on each system and each vector is the same.
// check for non-existent properties when loading from json.
// load to lua
// particle_sys
// spaces/scenes implemented as id-s only and operations on objects related to spaces/scenes (move, remove, etc).
// TODO(maybe):
// remove passive-like members because they are not used ??
// add_object ONLY on objects_sys, and pass an object_data const& (a base class) to it ??


// example of a simple update/draw loop with the new data oriented ECS
// someWindow.Clear();
// for (object const& o : objects.active)
// {
//      physics.Update(o, deltaTime);
//      animations.Update(o, deltaTime);
//      if (tnt::doo::object const s{sprites.draw_queue.size()};
//          s > obj && sprites.draw_queue[obj] != -1)
//          painter.Draw(o, someWindow);
// }
// someWindow.Render();


// example of a .json chunk containing objects data
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
    using object = std::size_t; /// < A data type that serves as a unique id of an object.

    /// @brief A struct that holds the basic data of an object.
    struct object_data
    {
        /// @brief Create a new object data handle from the given params.
        /// @param angle_ The initial angle of the object.
        /// @param scale_ The initial scale of the object.
        /// @param pos_ The initial position of the object.
        inline constexpr object_data(float angle_, Vector const &pos_, Vector const &scale_) noexcept
            : angle{angle_}, pos{pos_}, scale{scale_} {}

        float angle;  /// < The angle of the object.
        Vector scale; /// < The scale of the object.
        Vector pos;   /// < The position of the object.
    };

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
                passive.reserve(5);
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

        /// @brief Get the data of the object with the given id.
        /// @param id The id of the object.
        /// @return object_data
        inline object_data get_data(object id) const noexcept
        {
            return object_data{angle[id], pos[id], scale[id]};
        }

        /// @brief Load objects data from a json chunk.
        /// @param j The json chunk that contains the objects data.
        inline void from_json(nlohmann::json const &j)
        {
            add_object({j["angle"].get<float>(),
                        j["pos"].get<Vector>(),
                        j["scale"].get<Vector>()});
        }

        std::vector<object> active; /// < The id-s of all the active objects.

        std::vector<object> passive; /// < The id-s of all the non-active objects.

        std::vector<float> angle;  /// < The angles of the objects.
        std::vector<Vector> scale; /// < The scales of the objects.
        std::vector<Vector> pos;   /// < The positions of the objects.
    } objects;                     /// < An instance of objects_sys.
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_BASE_HPP