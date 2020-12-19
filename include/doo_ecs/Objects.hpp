#ifndef TNT_DOO_ECS_BASE_OBJECTS_SYSTEM_HPP
#define TNT_DOO_ECS_BASE_OBJECTS_SYSTEM_HPP

#include <nlohmann/json_fwd.hpp>

#include "core/Window.hpp"
#include "doo_ecs/Base.hpp"
#include "math/Vector.hpp"
#include "types/SparseSet.hpp"

namespace tnt::doo
{
    /// @brief A struct that holds the basic data of an object.
    struct object_data final
    {
        /// @brief Create a new object data handle from the given params.
        /// @param angle_ The initial angle of the object.
        /// @param scale_ The initial scale of the object.
        /// @param pos_ The initial position of the object.
        /// @param parent_ The parent of the new object. Leave it as is if you don't want the object to have a parent.
        constexpr object_data(float angle_, Vector const &pos_, Vector const &scale_,
                              object const &parent_ = null) noexcept
            : angle{angle_}, pos{pos_}, scale{scale_}, parent{parent_} {}

        float angle;   /// < The angle of the object.
        Vector scale;  /// < The scale of the object.
        Vector pos;    /// < The position of the object.
        object parent; /// < The parent of the object.
    };

    /// @brief A struct that handles objects data.
    inline struct objects_sys final
    {
        // objects_sys() = default;

        // objects_sys(objects_sys const &) = delete;
        // objects_sys &operator=(objects_sys const &) = delete;

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
        /// @return The id of the created object.
        object from_json(nlohmann::json const &j);

        /// @brief Store general data of a specific object to a json chunk.
        /// @param id The id of the object to serialize to json.
        /// @param j The json chunk where the data will be saved.
        void to_json(object const &id, nlohmann::json &j) ;

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

        /// @brief Remove the desired object from the objects system.
        /// @param id The id of the object you want to remove.
        /// @note Please call remove() from the other systems first.
        void remove(object const &id) noexcept;

        /// @brief Remove all the objects from the objects system.
        /// @note Please call clear() from the other systems first.
        void clear() noexcept;

        std::vector<float> angle; /// < The angles of the objects.

        tnt::sparse_set<object> active; /// < The id-s of all the active objects.
        std::vector<object> parent;     /// < The id-s of the parents of the objects.

        std::vector<Vector> scale; /// < The scales of the objects.
        std::vector<Vector> pos;   /// < The positions of the objects.
    } objects;                     /// < An instance of objects_sys.
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_BASE_OBJECTS_SYSTEM_HPP