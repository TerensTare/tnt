#ifndef TNT_MIRROR_REFLECTION_HPP
#define TNT_MIRROR_REFLECTION_HPP

#include "utils/TypeUtils.hpp"

// TODO:
// default constructible and argument constructible mirror::type
// remove namespace mirror::detail
// member reflection (is it needed ??)
// deal with references and pointers

namespace tnt::mirror
{
    namespace detail
    {
        template <typename T>
        concept copyable = std::is_copy_constructible_v<T>;

        template <typename T>
        concept movable = std::is_move_constructible_v<T>;

        template <typename T>
        concept default_constructible = std::is_default_constructible_v<T>;

        template <typename T>
        concept nothrow_copyable = std::is_nothrow_copy_constructible_v<T>;

        template <typename T>
        concept nothrow_movable = std::is_nothrow_move_constructible_v<T>;

        template <typename T>
        concept nothrow_default_constructible = std::is_nothrow_default_constructible_v<T>;
    } // namespace detail

    template <typename T, typename V>
    struct type : crtp<T>
    {
        inline explicit(!detail::default_constructible<V>) type() noexcept(detail::nothrow_default_constructible<V>)
            : value{} {}

        inline explicit(!detail::movable<V>) type(V &&v) noexcept(detail::nothrow_movable<V>)
            : value{std::move(v)} {}

        inline explicit(!detail::copyable<V>) type(V const &v) noexcept(detail::nothrow_copyable<V>)
            : value{v} {}

        inline constexpr char const *name() const noexcept { return this->base().name(); }

        V value;
    };

    // used for registering members of a type. NOTE: not finished yet
    template <typename T, char const *Name, auto &&... Args>
    struct type_register
        : type<type_register<T, Name, Args...>, T>
    {
        inline constexpr char const *name() const noexcept { return Name; }
    };

} // namespace tnt::mirror

#endif //!TNT_MIRROR_REFLECTION_HPP