#ifndef TNT_MIRROR_REFLECTION_HPP
#define TNT_MIRROR_REFLECTION_HPP

#include <map>
#include <variant>
#include <span>

#include "utils/Assert.hpp"
#include "utils/TypeUtils.hpp"

// TODO:
// default constructible and argument constructible mirror::type
// remove namespace mirror::detail
// member reflection (is it needed ??)
// deal with references and pointers
// support ctors, dtors, properties, templates (??), etc
// construct a new type from sth like:
// mirror.create<T>("key1", someVal, ...);
// find a way to call reflect only once for each type (maybe) by using a span ??

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

        // types
        template <typename T>
        concept class_t = std::is_class_v<T>;

        template <typename T>
        concept union_t = std::is_union_v<T>;

        template <typename T>
        concept enum_t = std::is_enum_v<T>;

        template <typename T>
        concept invocable = std::is_invocable_v<T>;

        template <typename T>
        concept fundamental_t = std::is_fundamental_v<T>;

        template <typename T>
        concept array_t = std::is_array_v<T>;

        template <typename T>
        concept member = std::is_member_object_pointer_v<T>;
    } // namespace detail

    struct class_tag_t
    {
    };
    struct enum_tag_t
    {
    };
    struct union_tag_t
    {
    };
    struct invocable_tag_t
    {
    };
    struct array_tag_t
    {
    };

    using type_varint = std::variant<class_tag_t, enum_tag_t, union_tag_t>; // TODO: add all types

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

        inline constexpr size_t size() const noexcept { return sizeof(V); }

        V value;
    };

    template <detail::class_t T>
    struct class_type : type<class_type<T>, T>
    {
        explicit class_type() noexcept = default;

        inline explicit class_type(std::map<char const *, void *> const &data_) noexcept
            : data{data_} {}

        inline constexpr char const *name() const noexcept { return typeid(T).name(); }

        inline class_type<T> reflect(
            char const *Name, auto const T::*Arg)
        {
            data[Name] = Arg;
            return *this;
        }

        inline class_type<T> create(
            std::span<std::pair<char const *, void const *>> args)
        {
            for (auto const &it : args)
                data[it.first] = const_cast<void *>(it.second);
            return *this;
        }

        template <typename R>
        [[nodiscard]] inline R *operator[](std::string_view key)
        {
            [[likely]] if (auto const it{data.find(key.data())};
                           it != data.cend()) return static_cast<R *>(it->second);
            safe_ensure(false, "Trying to access non-serialized member of class!!");
            return nullptr;
        }

    private:
        std::map<char const *, void *> data;
    };

    template <detail::enum_t T>
    struct enum_type : type<enum_type<T>, T>
    {
        explicit enum_type() noexcept = default;

        inline constexpr char const *name() const noexcept { return typeid(T).name(); }

        inline enum_type<T> reflect(char const *Name, T const &Arg)
        {
            data[Name] = Arg;
            return *this;
        }

        [[nodiscard]] inline T operator[](std::string_view key)
        {
            [[likely]] if (auto const it{data.find(key.data())};
                           it != data.cend()) return it->second;
            safe_ensure(false, "Trying to access non-serialized member of class!!");
            return T{0};
        }

    private:
        // (maybe) use a vector ??
        std::map<char const *, T> data;
    };

    // used for registering members of a type. NOTE: not finished yet
    struct mirror_t final
    {
        explicit inline constexpr mirror_t() noexcept = default;

        template <detail::class_t T>
        inline auto reflect(char const *Name, auto &&Arg)
        {
        }

        template <detail::enum_t T>
        inline auto reflect(char const *Name, auto &&Arg)
        {
        }
    };

} // namespace tnt::mirror

namespace tnt
{
    using mirror::mirror_t;
} // namespace tnt

#endif //!TNT_MIRROR_REFLECTION_HPP