#ifndef TNT_DOO_ECS_SYSTEM_UTILITIES_HPP
#define TNT_DOO_ECS_SYSTEM_UTILITIES_HPP

#include <concepts>
#include <nlohmann/json_fwd.hpp>

#include "doo_ecs/Base.hpp"
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

    struct system_type
    {
    };

    // clang-format off
    template <typename T>
    concept system = std::is_base_of_v<system_type, T>
        and requires { typename T::component; }
        and requires (T &t, object const& o, nlohmann::json& j,
            typename T::component const& c) {
            { t.add_object(o, c) } -> detail::void_type;
            { t.remove(o) } -> detail::void_type;
            { t.clear() } -> detail::void_type;
            { t.Update(o, 1.f) } -> detail::void_type;

            { t.from_json(o, j) } -> detail::void_type;
            { t.to_json(o, j) } -> detail::void_type;

            // TODO: draw_imgui;

            { t.active } -> std::same_as<std::vector<typename T::component>>;

        };
    // clang-format on

    template <system T>
    struct system_base : crtp<T>, system_type
    {
        static_assert(std::is_class_v<D> and std::is_same_v<D, std::remove_cv_t<D>>);
        static_assert(std::is_base_of_v<system_base<T>>, T >, "system T must derive from tnt::doo::system_base<T>!!");

        using component = typename T::component;

        inline void add_object(object const &id, component const &c)
        {
            this->super.add_object(id, c);
        }

        inline void Update(object const &id, float time_)
        {
            this->super.Update(id, time_);
        }

        inline void remove(object const &id)
        {
            this->super.remove(id);
        }

        inline void clear() noexcept(noexcept(super.clear())) { this->super.clear(); }

        inline void from_json(object const &id, nlohmann::json const &j)
        {
            this->super.from_json(id, j);
        }

        inline void to_json(object const &id, nlohmann::json &j)
        {
            this->super.to_json(id, j);
        }

        // TODO: draw_imgui
    };
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_SYSTEM_UTILITIES_HPP