#ifndef TNT_DOO_ECS_OBJECTS_HPP
#define TNT_DOO_ECS_OBJECTS_HPP

#include "utils/Assert.hpp"

#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Sprites.hpp"

#include "json/JsonDooObject.hpp"

// TODO:
// optimizations (ex. use pmr)
// ensure objects index on each system and each vector is the same.
// (maybe)
// add_object ONLY on objects_sys, and pass an object_params const& (a base class) to it.
// load from/to json files
// load to lua
// (maybe):
// remove the adl_serializer<object_data> ??

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
    class objects_sys final
    {
    public:
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

        inline void Update(object id, long long time_) noexcept
        {
            check(time_ > 0, "Calling tnt::doo::objects_sys::Update with parameter time_ begin 0. Objects will not be moved");
            pos[id] = pos[id] + (physics.phys[id].vel * float(time_ / 1000));
        }

        inline void Draw(object id, Window const &win) noexcept
        {
            float const dx{sprites.sprite[id].crop.w * scale[id].x * .5f};
            float const dy{sprites.sprite[id].crop.h * scale[id].y * .5f};
            SDL_FRect const dst{pos[id].x - dx, pos[id].y - dy, 2 * dx, 2 * dy};

            SDL_RenderCopyExF(
                win.getRenderer(), sprites.sprite[id].tex,
                &sprites.sprite[id].crop,
                &dst, angle[id], nullptr, SDL_FLIP_NONE);
        }

        inline object_data get_data(object id) const noexcept
        {
            return object_data{angle[id], scale[id], pos[id]};
        }

        inline void from_json(nlohmann::json const &j)
        {
            add_object(j.get<object_data>());
        }

        std::vector<object> active;

        std::vector<object> passive;

        std::vector<float> angle;
        std::vector<Vector> scale;
        std::vector<Vector> pos;
    } objects;
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_OBJECTS_HPP