#ifndef TNT_EXCEPTIONLESS_EXCEPTED_HPP
#define TNT_EXCEPTIONLESS_EXCEPTED_HPP

#include <compare>
#include <functional>
#include <memory>

// .wrap_error (or error_or) or similar, that prints an error if it happens or does the other thing otherwise

namespace tnt
{
    namespace detail
    {
        struct valid_t final
        {
            explicit constexpr valid_t(int) noexcept {}
        };
    }

    inline static constexpr detail::valid_t valid{42};

    template <std::movable E>
    struct [[nodiscard]] unexpected final
    {
        static_assert(not std::is_reference_v<E>, "Cannot pass references to tnt::unexpected!! "
                                                  "Please consider using tnt::unexpected<std::reference_wrapper<T>> or similar.");

        using value_type = E;

        explicit constexpr unexpected(E &&err) noexcept
            : errc{std::forward<E &&>(err)} {}

        [[nodiscard]] constexpr E &value() &noexcept { return errc; }
        [[nodiscard]] constexpr E const &value() const &noexcept { return errc; }
        [[nodiscard]] constexpr E &&value() &&noexcept { return std::move(errc); }
        [[nodiscard]] constexpr E const &&value() const &&noexcept { return std::move(errc); }

    private:
        E errc;
    };

    template <std::movable E>
    unexpected(E &&) -> unexpected<E>;

    template <typename T, typename E>
    struct [[nodiscard]] expected final
    {
        using value_type = T;
        using error_type = E;

        expected() = delete;
        expected(expected const &) = default;
        expected &operator=(expected const &) = default;

        // deleted, create from unexpected<E> instead
        constexpr expected(E) = delete;

        constexpr expected(T &&val) noexcept(std::is_move_constructible_v<T>)
            : valid_{true}, value_{std::forward<T>(val)} {}

        constexpr expected(unexpected<E> &&rhs) noexcept
            : valid_{false}, error_{std::forward<E>(rhs.value())} {}

        constexpr expected(expected &&rhs) noexcept(std::is_move_constructible_v<T>) requires std::is_move_constructible_v<T>
            : valid_{std::exchange(rhs.valid_, false)}
        {
            if (valid_)
                std::construct_at(std::addressof(value_), std::move(rhs.value_));
            else
                std::construct_at(std::addressof(error_), rhs.error_);
        }

        // deleted, assign from unexpected<E> instead
        constexpr expected &operator=(E) = delete;

        constexpr expected &operator=(expected &&rhs) noexcept(std::is_move_constructible_v<T>) requires(std::is_move_constructible_v<T> &&std::swappable<T> &&std::swappable<E>)
        {
            if (valid_ && rhs.valid_)
                std::swap(value_, rhs.value_);
            else if (valid_ && !rhs.valid_)
            {
                std::destroy_at(std::launder(std::addressof(value_)));
                std::construct_at(std::addressof(error_), rhs.error_);
                valid_ = false;
            }
            else if (!valid_ && rhs.valid_)
            {
                std::destroy_at(std::launder(std::addressof(error_)));
                std::construct_at(std::addressof(value_), rhs.value_);
                valid_ = true;
            }
            else
                std::swap(error_, rhs.error_);
            return *this;
        }

        constexpr ~expected() noexcept
        {
            if (valid_)
                std::destroy_at(std::launder(std::addressof(value_)));
            else
                std::destroy_at(std::launder(std::addressof(error_)));
        }

        // clang-format off
        template <typename Success, typename Error>
            requires (std::invocable<Success &&, T &&> && std::invocable<Error &&, E &&>)
        constexpr void match(Success &&success_fn, Error &&error_fn) &&
        {
            // clang-format on
            if (valid_)
                std::invoke(std::forward<Success &&>(success_fn), std::move(value_));
            else
                std::invoke(std::forward<Error &&>(error_fn), std::move(error_));
        }

        // clang-format off
        template <typename Success, typename Error>
            requires (std::invocable<Success &&, T &> && std::invocable<Error &&, E &>)
        constexpr void match(Success &&success_fn, Error &&error_fn) &
        {
            // clang-format on
            if (valid_)
                std::invoke(std::forward<Success &&>(success_fn), value_);
            else
                std::invoke(std::forward<Error &&>(error_fn), error_);
        }

        // clang-format off
        template <typename Success, typename Error>
            requires (std::invocable<Success &&, T const &> && std::invocable<Error &&, E const &>)
        constexpr void match(Success &&success_fn, Error &&error_fn) const &
        {
            // clang-format on
            if (valid_)
                std::invoke(std::forward<Success &&>(success_fn), value_);
            else
                std::invoke(std::forward<Error &&>(error_fn), error_);
        }

        constexpr bool valid() const noexcept { return valid_; }
        /* explicit */ constexpr operator bool() const noexcept { return valid_; }

        [[nodiscard]] constexpr T &error() &noexcept { return error_; }
        [[nodiscard]] constexpr T const &error() const &noexcept { return error_; }
        [[nodiscard]] constexpr T &&error() &&noexcept { return std::move(error_); }
        [[nodiscard]] constexpr T const &&error() const &&noexcept { return std::move(error_); }

        [[nodiscard]] constexpr T &value() &noexcept { return value_; }
        [[nodiscard]] constexpr T const &value() const &noexcept { return value_; }
        [[nodiscard]] constexpr T &&value() &&noexcept { return std::move(value_); }
        [[nodiscard]] constexpr T const &&value() const &&noexcept { return std::move(value_); }

        // clang-format off
        template <typename U = T>
            requires std::constructible_from<T, U>
        [[nodiscard]] constexpr T value_or(U &&u) const &noexcept { return (valid_) ? value_ : std::forward<U>(u); }

        template <typename U = T>
            requires std::constructible_from<T, U>
        [[nodiscard]] constexpr T value_or(U &&u) &&noexcept { return (valid_) ? std::move(value_) : std::forward<U>(u); }

        template <typename Fn>
            requires std::invocable<Fn &&, T &&>
        constexpr std::invoke_result_t<Fn &&, T &&> map(Fn &&fn) &&noexcept(
            std::is_nothrow_invocable_v<Fn &&, T &&>)
        {
            // clang-format on
            if (valid_)
                return std::invoke(std::forward<Fn &&>(fn), std::move(value_));
        }

        // clang-format off
        template <typename Fn>
            requires std::invocable<Fn &&, T &>
        constexpr std::invoke_result_t<Fn &&, T &> map(Fn &&fn) &noexcept(
            std::is_nothrow_invocable_v<Fn &&, T &>)
        {
            // clang-format on
            if (valid_)
                return std::invoke(std::forward<Fn &&>(fn), value_);
        }

        // clang-format off
        template <typename Fn>
            requires std::invocable<Fn &&, T const &>
        constexpr std::invoke_result_t<Fn &&, T const &> map(Fn &&fn) const &noexcept(
            std::is_nothrow_invocable_v<Fn &&, T const &>)
        {
            // clang-format on
            if (valid_)
                return std::invoke(std::forward<Fn &&>(fn), value_);
        }

        // clang-format off
        template <typename Fn>
            requires std::invocable<Fn &&, E &&>
        constexpr std::invoke_result_t<Fn &&, E &&> map_error(Fn &&fn) &&noexcept(
            std::is_nothrow_invocable_v<Fn &&, E &&>)
        {
            // clang-format on
            if (valid_)
                return std::invoke(std::forward<Fn &&>(fn), std::move(error_));
        }

        // clang-format off
        template <typename Fn>
            requires std::invocable<Fn &&, E &>
        constexpr std::invoke_result_t<Fn &&, E &> map_error(Fn &&fn) &noexcept(
            std::is_nothrow_invocable_v<Fn &&, E &>)
        {
            // clang-format on
            if (valid_)
                return std::invoke(std::forward<Fn &&>(fn), error_);
        }

        // clang-format off
        template <typename Fn>
            requires std::invocable<Fn &&, E const &>
        constexpr std::invoke_result_t<Fn &&, E const &> map_error(Fn &&fn) const &noexcept(
            std::is_nothrow_invocable_v<Fn &&, E const &>)
        {
            // clang-format on
            if (valid_)
                return std::invoke(std::forward<Fn &&>(fn), error_);
        }

        template <std::swappable_with<T> U = T, std::swappable_with<E> F = E>
        constexpr void swap(expected<U, F> &rhs) noexcept(std::is_nothrow_swappable_with_v<T, U> &&std::is_nothrow_swappable_with_v<E, F>)
        {
            using std::swap;
            if (valid_ && rhs.valid_)
                swap(value_, rhs.value_);
            else if (valid_ && !rhs.valid_)
            {
                std::construct_at(std::addressof(error_), rhs.error_);
                std::destroy_at(std::launder(std::addressof(rhs.error_)));
                std::construct_at(std::addressof(rhs.value_), value_);
                std::destroy_at(std::launder(std::addressof(value_)));
            }
            else if (!valid_ && rhs.valid_)
            {
                std::construct_at(std::addressof(rhs.error_), error_);
                std::destroy_at(std::launder(std::addressof(error_)));
                std::construct_at(std::addressof(value_), rhs.value_);
                std::destroy_at(std::launder(std::addressof(rhs.value_)));
            }
            else
                swap(error_, rhs.error_);

            swap(valid_, rhs.valid_);
        }

        template <std::swappable_with<T> U = T, std::swappable_with<E> F = E>
        friend constexpr void swap(expected<T, E> &lhs, expected<U, F> &rhs) noexcept(std::is_nothrow_swappable_with_v<T, U> &&std::is_nothrow_swappable_with_v<E, F>)
        {
            lhs.swap(rhs);
        }

        // template <std::equality_comparable_with<T> U = T>
        // friend constexpr bool operator==(expected<T, E> const& lhs, U const& rhs)
        // {
        //     return lhs.valid_ && (lhs.value_ == rhs);
        // }

        // template <std::three_way_comparable_with<T> U = T>
        // friend constexpr auto operator<=>(expected<T, E> const& lhs, U const& rhs)
        //     -> std::compare_three_way_result_t<T, U>
        // {
        //     if (lhs.valid_)
        //         return lhs.value_ <=> rhs;
        //     return std::compare_three_way_result_t<T, U>::less;
        // }

        template <std::equality_comparable_with<T> U = T, std::equality_comparable_with<E> F = E>
        friend constexpr bool operator==(expected<T, E> const &lhs, expected<U, F> const &rhs)
        {
            if (lhs.valid_ != rhs.valid_) // cannot compare a valid expected with an invalid expected
                return false;
            if (lhs.valid_) // if not in different states, it means they are both valid
                return (lhs.value_ == rhs.value_);
            else // or both invalid
                return (lhs.error_ == rhs.error_);
        }

        template <std::three_way_comparable_with<T> U, std::three_way_comparable_with<E> F>
        friend constexpr auto operator<=>(expected<T, E> const &lhs, expected<U, F> const &rhs)
            -> std::common_comparison_category_t<
                std::compare_three_way_result_t<T, U>,
                std::compare_three_way_result_t<E, F>>
        {
            if (auto const cmp = lhs.valid_ <=> rhs.valid_; cmp != 0) // cannot compare a valid expected with an invalid expected
                return cmp;
            if (lhs.valid_) // if not in different states, it means they are both valid
                return (lhs.value_ <=> rhs.value_);
            else // or both invalid
                return (lhs.error_ <=> rhs.error_);
        }

    private:
        union
        {
            T value_;
            E error_;
        };
        bool valid_;
    };

    template <typename E>
    struct [[nodiscard]] expected<void, E>
    {
        using value_type = void;
        using error_type = E;

        constexpr expected(detail::valid_t) noexcept
            : null{detail::valid_t{42}}, valid_{true} {}
        expected(expected const &) = delete;
        expected &operator=(expected const &) = delete;

        // deleted, create from unexpected<E> instead
        constexpr expected(E) = delete;

        constexpr expected(unexpected<E> &&rhs) noexcept
            : valid_{false}, error_{std::forward<E>(rhs.value())} {}

        constexpr expected(expected &&rhs) noexcept
            : valid_{std::exchange(rhs.valid_, false)}
        {
            if (!valid_)
                std::construct_at(std::addressof(error_), rhs.error_);
        }

        // deleted, assign from unexpected<E> instead
        constexpr expected &operator=(E) = delete;

        constexpr expected &operator=(expected &&rhs) noexcept
            requires std::swappable<E>
        {
            if (valid_ && !rhs.valid_)
            {
                std::construct_at(std::addressof(error_), rhs.error_);
                valid_ = false;
            }
            else if (!valid_ && rhs.valid_)
            {
                std::destroy_at(std::launder(std::addressof(error_)));
                null = detail::valid_t{42};
                valid_ = true;
            }
            else
                std::swap(error_, rhs.error_);
            return *this;
        }

        constexpr ~expected() noexcept
        {
            if (!valid_)
                std::destroy_at(std::launder(std::addressof(error_)));
        }

        // clang-format off
        template <typename Error>
            requires std::invocable<Error &&, E &&>
        constexpr void match(Error &&error_fn) &&
        {
            // clang-format on
            if (!valid_)
                std::invoke(std::forward<Error &&>(error_fn), std::move(error_));
        }

        // clang-format off
        template <typename Error>
            requires std::invocable<Error &&, E &>
        constexpr void match(Error &&error_fn) &
        {
            // clang-format on
            if (!valid_)
                std::invoke(std::forward<Error &&>(error_fn), error_);
        }

        // clang-format off
        template <typename Error>
            requires std::invocable<Error &&, E const &>
        constexpr void match(Error &&error_fn) const &
        {
            // clang-format on
            if (!valid_)
                std::invoke(std::forward<Error &&>(error_fn), error_);
        }

        constexpr bool valid() const noexcept { return valid_; }
        /* explicit */ constexpr operator bool() const noexcept { return valid_; }

    private:
        union
        {
            E error_;
            detail::valid_t null;
        };
        bool valid_;
    };
}

#endif //!TNT_EXCEPTIONLESS_EXCEPTED_HPP