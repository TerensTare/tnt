#ifndef TNT_POLYMORPHIC_RESOURCE_HPP
#define TNT_POLYMORPHIC_RESOURCE_HPP

#include <memory_resource>

// TODO:
// add allocator to the ctor checks.

namespace tnt
{
    template <typename T, std::size_t Buffer,
              typename R = std::pmr::monotonic_buffer_resource>
    struct poly_resource final
    {
        // clang-format off
        template <typename... Args>
        requires std::constructible_from<T, Args...>
        inline explicit(sizeof...(Args) != 1) poly_resource(Args &&... args)
        noexcept(std::is_nothrow_constructible_v<T, Args...>)
            : resource{std::forward<Args>(args)..., &res} {}
        // clang-format on

        inline poly_resource(poly_resource<T, Buffer> const &other) requires std::copy_constructible<T>
            : resource{other.resource} {}

        inline poly_resource(poly_resource<T, Buffer> &&other) requires std::move_constructible<T>
            : resource{std::move(other.resource)} {}

        template <std::size_t S2>
        requires { S2 <= Buffer; }
        inline poly_resource(poly_resource<T, S2> const &other) requires std::copy_constructible<T>
            : resource{other.resource} {}

        template <std::size_t S2>
        requires { S2 <= Buffer; }
        inline poly_resource(poly_resource<T, S2> &&other) requires std::move_constructible<T>
            : resource{std::move(other.resource)} {}

        inline poly_resource &operator=(poly_resource<T, Buffer> const &other) requires std::copyable<T>
        {
            resource = other.resource;
        }

        inline poly_resource &operator=(poly_resource<T, Buffer> &&other) requires std::movable<T>
        {
            resource = std::move(other.resource);
        }

        inline T &operator*() requires std::copyable<T>
        {
            return resource.data;
        }

        inline T &operator->() requires std::copyable<T>
        {
            return resource.data;
        }

        inline const T &operator*() const requires std::copyable<T>
        {
            return resource.data;
        }

        inline const T &operator->() const requires std::copyable<T>
        {
            return resource.data;
        }

        inline T &get() requires std::copyable<T>
        {
            return resource.data;
        }

        inline const T &get() const requires std::copyable<T>
        {
            return resource.data;
        }

    private:
        static_assert(std::derived_from<std::pmr::memory_resource, R>,
                      "Invalid memory resource type!!");

        struct s1 final
        {
            // clang-format off
            template <typename... Args>
            requires std::constructible_from<T, Args...>
            inline explicit(sizeof...(Args) != 1) poly_resource(Args &&... args)
            noexcept(std::is_nothrow_constructible_v<T, Args...>)
            : data{std::forward<Args>(args)..., &res} {}
            // clang-format on

            std::byte buffer[Buffer];
            R res{buffer, Buffer};
            T data;
        };

        struct s2 final
        {
            // clang-format off
            template <typename... Args>
            requires std::constructible_from<T, Args...>
            inline explicit(sizeof...(Args) != 1) poly_resource(Args &&... args)
            noexcept(std::is_nothrow_constructible_v<T, Args...>)
            : data{std::forward<Args>(args)..., &res} {}
            // clang-format on

            T data;
            std::byte buffer[Buffer];
            R res{buffer, Buffer};
        };

        std::conditional_t<(sizeof(T) < sizeof(std::byte) * Buffer), s2, s1> resource;
    };
} // namespace tnt

#endif //!TNT_POLYMORPHIC_RESOURCE_HPP