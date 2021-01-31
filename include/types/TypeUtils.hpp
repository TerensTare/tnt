#ifndef TNT_TYPE_UTILS_HPP
#define TNT_TYPE_UTILS_HPP

#include "utils/TypeLists.hpp"

namespace tnt
{
    template <typename... Ts>
    struct overload final : Ts...
    {
        using Ts::operator()...;
    };

    template <typename... Ts>
    overload(Ts...) -> overload<Ts...>;

    // thx riptutorial
    // https://riptutorial.com/cplusplus/example/8508/recursive-lambdas
    /// @brief A helper class for defining a y combinator, which helps on defining recursive functions.
    /// @tparam Fn The type of the recursive function.
    /// @note If you are using the class with lambdas, you have to specify the return type of the lambda. The same applies for functions, so you can't use y_comb with functions that have `auto` return type.
    template <typename Fn>
    class y_comb
    {
    public:
        /// @brief Create a new recursive function using an y combinator.
        /// @param fn_ The recursive function helper.
        explicit constexpr y_comb(Fn &&fn_) noexcept(std::is_nothrow_move_constructible_v<Fn>)
            : fn{std::move(fn_)} {}

        /// @brief Call the function with the given @a args...
        template <typename... Args>
        // clang-format off
            requires std::invocable<Fn, y_comb<Fn>&, Args...>
        constexpr decltype(auto) operator()(Args &&... args) const
            noexcept(std::is_nothrow_invocable_v<Fn, y_comb<Fn>&, Args...>)
        {
            // clang-format on
            return fn(*this, std::forward<Args>(args)...);
        }

    private:
        /*[[no_unique_address]]*/ Fn fn;
    };

    template <typename Fn>
    y_comb(Fn &&) -> y_comb<std::decay_t<Fn>>;

    // TODO: ensure ret can multiply an int
    inline constexpr auto if_then = [](bool cond, auto const &ret) {
        return (ret * cond);
    };

    inline constexpr auto if_else = [](bool cond, auto const &r1, auto const &r2) {
        static_assert(std::is_same_v<decltype(r1), decltype(r2)> ||
                          std::is_convertible_v<decltype(r1), decltype(r2)> ||
                          std::is_convertible_v<decltype(r2), decltype(r1)>,
                      "Parameters passed to if_else cannot be converted to each-other's type!!");
        return (r1 * cond + r2 * (!cond));
    };

    struct non_copyable
    {
        non_copyable() = default;
        non_copyable(non_copyable const &) = delete;
        non_copyable &operator=(non_copyable const &) = delete;
    };

    struct non_movable
    {
        non_movable() = default;
        non_movable(non_movable &&) = delete;
        non_movable &operator=(non_movable &&) = delete;
    };

    template <typename T, typename Deleter = std::default_delete<T>>
    struct singleton : non_copyable
    {
        ~singleton() noexcept(noexcept(Deleter{}())) { Deleter{}(); }

        static T &This() noexcept(std::is_nothrow_constructible_v<T>)
        {
            static T inst;
            return inst;
        }
    };

    // thx Jonathan Boccara
    // https://www.fluentcpp.com/2017/05/19/crtp-helper/
    template <typename T>
    struct crtp
    {
        constexpr T &base() noexcept { return static_cast<T &>(*this); }
        constexpr T const &base() const noexcept
        {
            return static_cast<T const &>(*this);
        }
    };

    template <typename>
    struct template_traits;

    template <template <typename...> typename T, typename... Args>
    struct template_traits<T<Args...>>
    {
        template <std::size_t Index>
        requires requires { Index < sizeof...(Args); }
        using arg_t = tl::at_t<type_list<Args...>, Index>;
    };

    template <typename T, std::size_t I>
    using template_arg_t = typename template_traits<T>::template arg_t<I>;
} // namespace tnt

#endif //! TNT_TYPE_UTILS_HPP