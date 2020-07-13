#ifndef TNT_ECS_BASE_FWD_HPP
#define TNT_ECS_BASE_FWD_HPP

#include <type_traits>

namespace tnt
{
    class Object;
    class SpriteComponent;

    /// @brief The base class for all the Component types.
    class Component
    {
    };

    /// @brief A concept sowing the basic props a @c Component should have.
    template <typename T>
    concept component = std::is_base_of_v<Component, T>;

    /// @brief An @c Object that can be drawed.
    template <typename T>
    concept drawable = requires(T const *t)
    {
        {
            t->get<SpriteComponent>()
        }
        ->std::same_as<SpriteComponent *>;
    }
    &&std::is_base_of_v<Object, T>;

    /// @brief A basic @c Object type.
    template <typename T>
    concept object = std::is_base_of_v<Object, T>;
} // namespace tnt

#endif //!TNT_ECS_BASE_FWD_HPP