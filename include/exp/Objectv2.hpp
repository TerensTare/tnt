#ifndef TNT_ECS_EXPERIMENTAL_OBJECT_HPP
#define TNT_ECS_EXPERIMENTAL_OBJECT_HPP

#include <type_traits>
#include "math/Vector.hpp"

// NOTE: everything here is fully experimental and coded on the go.
// This means that this file is subject to change or even be removed.

// problems with approach
// 1. Components can't be added/removed at runtime.
// 2. No good way to initialize Components individually (except with copy/move construction).
// 3. diamond inheritance.
// 4. Each Component needs to have move/copy ctor & assignment.

// TODO:
// is the constructor right ??

namespace tnt::exp
{
    class Component
    {
    };

    class TransformComp : public Component
    {
    public:
        inline explicit TransformComp(Vector const &v) noexcept : vec{v} {}

        inline void setPosition(Vector const &v) noexcept { vec = v; }
        inline Vector getPosition() const noexcept { return vec; }

        inline void Transform(Vector const &v) noexcept { vec = vec + v; }

    private:
        Vector vec;
    };

    template <typename T>
    concept component_v2 = std::is_base_of_v<Component, T>;

    template <component_v2... Components>
    class Object : public Components...
    {
    public:
        inline explicit Object(Components &&... comps) noexcept
            : Components{std::forward<Components>(comps)}... {}

        template <component_v2 Comp>
        inline constexpr bool has() noexcept { return std::is_base_of_v<Comp, decltype(this)>; }

    private:
        using Components::Components...;
    };

    template <component_v2... Comps>
    Object(Comps...)->Object<std::decay_t<Comps>...>;
} // namespace tnt::exp

#endif //!TNT_ECS_EXPERIMENTAL_OBJECT_HPP