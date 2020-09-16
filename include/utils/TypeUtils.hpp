#ifndef TNT_TYPE_UTILS_HPP
#define TNT_TYPE_UTILS_HPP

namespace tnt
{
    template <typename... Ts>
    struct overload final : Ts...
    {
        using Ts::operator()...;
    };

    template <typename... Ts>
    overload(Ts...) -> overload<Ts...>;

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
        ~singleton() noexcept(noexcept(Deleter())) { Deleter(); }

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
        inline T &base() noexcept { return static_cast<T &>(*this); }
        inline T const &base() const noexcept { return static_cast<T const &>(*this); }
    };
} // namespace tnt

#endif //! TNT_TYPE_UTILS_HPP