#ifndef TNT_ANY_MIRROR_HPP
#define TNT_ANY_MIRROR_HPP

#include "TypeInfo.hpp"
#include "utils/Assert.hpp"

namespace tnt
{
    template <typename>
    struct in_place_tag_t
    {
        explicit in_place_tag_t() = default;
    };
    template <typename T>
    inline constexpr in_place_tag_t<T> in_place_t{};

    class any final
    {
    public:
        constexpr any() noexcept
            : data{nullptr}, info{tnt::type_id<void>()} {}

        constexpr any(any const &other) noexcept
            : data{other.data}, info{other.info} {}

        constexpr any(any &&other) noexcept
            : data{std::exchange(other.data, nullptr)},
              info{std::exchange(other.info, tnt::type_id<void>())} {}

        template <std::movable T>
        inline any(T &&t) : info{tnt::type_id<T>()}
        {
            auto a = std::move(t);
            data = (void *)&a;
        }

        // clang-format off
        template <typename T, std::movable... Args>
        requires std::constructible_from<T, Args...> 
        inline explicit any(tnt::in_place_tag_t<T>, Args &&... args) noexcept(
            std::is_nothrow_constructible_v<T, Args...>)
            : data{std::forward<Args>(args)...}, info{tnt::type_id<T>()} {}
        // clang-format on

        inline any &operator=(any const &other)
        {
            any{other}.swap(*this);
            return *this;
        }

        inline any &operator=(any &&other) noexcept
        {
            other.swap(*this);
            any{}.swap(other);
            return *this;
        }

        template <std::movable T>
        inline any &operator=(T &&t)
        {
            any{std::forward<T>(t)}.swap(*this);
            return *this;
        }

        inline any &swap(any &other) noexcept
        {
            {
                void *tmp{data};
                data = other.data;
                other.data = tmp;
            }
            {
                tnt::type_info const &tmp{info};
                info = other.info;
                other.info = tmp;
            }

            return *this;
        }

        constexpr void reset() noexcept
        {
            data = nullptr;
            info = tnt::type_id<void>();
        }

        constexpr bool has_value() noexcept
        {
            return (info != tnt::type_id<void>());
        }

        constexpr const tnt::type_info &type() const noexcept { return info; }

        template <typename T>
        inline const T to() const noexcept
        {
            using U = const std::remove_cvref_t<T> &;
            static_assert(std::constructible_from<T, U>);
            safe_ensure(info == tnt::type_id<T>(),
                        "tnt::any: Casting to wrong type!!");

            using R = std::remove_reference_t<T>;
            R *res{static_cast<R *>(data)};
            safe_ensure(res, "tnt::any: Cast produced invalid data!!");

            using O = std::conditional_t<std::is_reference_v<T>, T, std::add_lvalue_reference_t<T>>;

            return static_cast<O>(*res);
        }

        template <typename T>
        inline T to() noexcept
        {
            using U = std::remove_cvref_t<T> &;
            static_assert(std::constructible_from<T, U>);
            safe_ensure(info == tnt::type_id<T>(),
                        "tnt::any: Casting to wrong type!!");

            using R = std::remove_reference_t<T>;
            R *res{static_cast<R *>(data)};
            safe_ensure(res, "tnt::any: cast produced invalid data!!");

            using O = std::conditional_t<std::is_reference_v<T>, T, std::add_lvalue_reference_t<T>>;

            return static_cast<O>(*res);
        }

    private:
        void *data;
        tnt::type_info info;
    };

    template <typename T>
    inline T any_cast(any &obj) noexcept(noexcept(obj.to<T>()))
    {
        return obj.to<T>();
    }

    template <typename T>
    inline T any_cast(any const &obj) noexcept(noexcept(obj.to<T>()))
    {
        return obj.to<T>();
    }

    template <typename T>
    inline T any_cast(any &&obj) noexcept(noexcept(obj.to<T>()))
    {
        static_assert(std::is_rvalue_reference_v<T &&> ||
                          std::is_const_v<std::remove_reference_t<T>>,
                      "Can't construct constant reference"
                      "from temporary tnt::any!!");
        return any_cast<T>(obj);
    }

    template <typename T, std::movable... Args>
    inline any make_any(Args &&... args) noexcept(
        std::is_nothrow_constructible_v<T, Args...>)
    {
        static_assert(std::constructible_from<T, Args...>,
                      "Inalid arguments passed tnt::make_any!!");
        return any{in_place_t<T>, std::forward<Args>(args)...};
    }
} // namespace tnt

#endif //!TNT_ANY_MIRROR_HPP