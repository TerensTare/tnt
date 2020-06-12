#ifndef TNT_ECS2_OBJECT_HPP
#define TNT_ECS2_OBJECT_HPP

#include "Utils.hpp"
#include "math/Vector.hpp"

// problems to solve
// 1. components can't be added/removed at runtime.
// 2. diamond inheritance.
// 3. need to write `using` on each derived class.
// 4. no CTAD for Object (yet).
// 5. sth like
// template<...> class MyObject : public Object<Comp1, .., CompN, ...> {};

// TODO:
// getParent/setParent.
// global angle/scale/position.

namespace tnt
{
    class Window;
}

namespace tnt::ecs2
{
    namespace detail
    {
        // thx Jonathan Boccara
        // https://www.fluentcpp.com/2017/05/19/crtp-helper/
        template <typename T>
        struct crtp
        {
            inline T &underlying() noexcept { return static_cast<T &>(*this); }
            inline T const &underlying() const noexcept { return static_cast<T const &>(*this); }
        };
    } // namespace detail

    class base_object
    {
    };

    /// @brief A class representing a basic component for game objects.
    class Component
    {
    };

    template <typename T>
    concept component = std::is_base_of_v<tnt::ecs2::Component, T>;

    template <typename T>
    concept object = std::is_base_of_v<base_object, T>;

    /// @brief A class representing a basic game object.
    /// @tparam Components... The components that the @c Object should have.
    template <typename T, component... Components>
    class Object
        : public base_object,
          public detail::crtp<T>,
          public Components...
    {
    public:
        inline explicit Object(Components &&... comps) noexcept
            : Components{std::forward<Components>(comps)}... {}

        template <component Comp>
        inline constexpr bool has() noexcept { return std::is_base_of_v<Comp, Object>; }

        inline void Update(long long time_) noexcept
        {
            this->underlying().Update(time_);
        }

        inline void Draw(tnt::Window const *win) noexcept
        {
            if constexpr (is_detected_v<decltype(typename T::Draw), T>::value)
                this->underlying().Draw(win);
        }
    };
} // namespace tnt::ecs2

#endif //!TNT_ECS_EXPERIMENTAL_OBJECT_HPP