#ifndef TNT_DOO_ECS_SYSTEM_UTILITIES_HPP
#define TNT_DOO_ECS_SYSTEM_UTILITIES_HPP

#include <concepts>
#include <nlohmann/json_fwd.hpp>

#include "doo_ecs/Base.hpp"

#include "types/SparseSet.hpp"
#include "types/TypeUtils.hpp"

// TODO:
// noexcept
// draw_imgui
// if the system provides remove and active, but not clear, declare clear to be remove on each object

// TODO(maybe):
// store base() in a member reference ??

namespace tnt::doo
{
    namespace detail
    {
        template <typename T>
        concept void_type = std::is_void_v<T>;
    } // namespace detail

    // clang-format off
    /// @brief A concept representing the minimal requirements that a type must fulfill to be a DOO ECS system.
    template <typename T>
    concept system = requires { typename T::component; }
        and requires (T &t, object const& o, nlohmann::json& j,
            typename T::component const& c) {
            { t.add_object(o, c) } -> detail::void_type;
            { t.remove(o) } -> detail::void_type;
            { t.clear() } -> detail::void_type;
            { t.Update(o, 1.f) } -> detail::void_type;

            { t.from_json(o, j) } -> detail::void_type;
            { t.to_json(o, j) } -> detail::void_type;

            // TODO: draw_imgui;

            { t.active } -> std::same_as<tnt::sparse_set<tnt::doo::object>>;

        };
    // clang-format on

    /// @brief A helper class for defining custom DOO ECS systems.
    /// @tparam T The class that will be used as a system.
    template <system T>
    struct system_base : crtp<T>
    {
        static_assert(std::is_class_v<T> and std::is_same_v<T, std::remove_cv_t<T>>);
        static_assert(std::is_base_of_v<system_base<T>, T>, "system T must derive from tnt::doo::system_base<T>!!");

        /// @brief The component type stored by this system.
        using component = typename T::component;

        /// @brief Add the data of the object with given id to the system.
        /// @param id The id of the object.
        /// @param c The component data.
        inline void add_object(object const &id, component const &c)
        {
            this->super.add_object(id, c);
        }

        /// @brief Update the data of the desired object.
        /// @param id The id of the object.
        /// @param time_ The time elapsed since the last Update call.
        inline void Update(object const &id, float time_)
        {
            this->super.Update(id, time_);
        }

        /// @brief Remove the data of the desired object from the system.
        /// @param id The id of the object to remove.
        inline void remove(object const &id)
        {
            this->super.remove(id);
        }

        /// @brief Remove the data of all the objects from the system.
        inline void clear() noexcept(noexcept(super.clear())) { this->super.clear(); }

        /// @brief Load the data of an object from a json chunk.
        /// @param id The id of the desired object.
        /// @param j The json chunk containing the data of the component.
        inline void from_json(object const &id, nlohmann::json const &j)
        {
            this->super.from_json(id, j);
        }

        /// @brief Serialize the component data of the given object to a json chunk.
        /// @param id The id of the desired object.
        /// @param j The json chunk where the data will be stored.
        inline void to_json(object const &id, nlohmann::json &j)
        {
            this->super.to_json(id, j);
        }

        // TODO: draw_imgui
    };
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_SYSTEM_UTILITIES_HPP