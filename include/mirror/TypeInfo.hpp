#ifndef TNT_TYPE_INFO_MIRROR_HPP
#define TNT_TYPE_INFO_MIRROR_HPP

#include <functional>

namespace tnt
{
    struct [[nodiscard]] type_info final
    {
        constexpr type_info(type_info const &other) noexcept
            : name_{other.name_}, index{other.index} {}

        constexpr type_info(type_info &&other) noexcept
            : name_{std::exchange(other.name_, {})},
              index{std::exchange(other.index, {})} {}

        constexpr type_info &operator=(type_info const &other) noexcept
        {
            name_ = other.name_;
            index = other.index;
            return *this;
        }

        constexpr type_info &operator=(type_info &&other) noexcept
        {
            if (this != &other)
            {
                name_ = std::exchange(other.name_, {});
                index = std::exchange(other.index, {});
            }
            return *this;
        }

        constexpr std::size_t hash() const noexcept { return index; }
        constexpr auto name() const noexcept { return name_; }

        [[nodiscard]] constexpr bool operator==(type_info const &other) const noexcept
        {
            return index == other.index;
        }

    private:
        constexpr type_info(std::string_view n, std::size_t const &h) noexcept
            : name_{n}, index{h} {}

        std::string_view name_;
        std::size_t index{};

        template <typename T>
        friend constexpr type_info type_id() noexcept;
    };

    inline std::ostream &operator<<(std::ostream &o, type_info const &t)
    {
        return (o << t.name());
    }

    template <typename T>
    [[nodiscard]] constexpr std::string_view type_name() noexcept
    {
        // thx Boost.UT authors
        // https://github.com/boost-ext/ut/blob/3b05dca6a629497910cf8e92aebcaead0124c8b4/include/boost/ut.hpp#L228
#if defined(_MSC_VER) and not defined(__clang__)
        return {__FUNCSIG__ + 120, sizeof(__FUNCSIG__) - 128};
#elif defined(__clang__)
        return {__PRETTY_FUNCTION__ + 39, sizeof(__PRETTY_FUNCTION__) - 41};
#elif defined(__GNUC__)
        return {__PRETTY_FUNCTION__ + 54, sizeof(__PRETTY_FUNCTION__) - 105};
#endif
    }

    template <typename T>
    constexpr type_info type_id() noexcept
    {
        std::size_t addr{};
        constexpr std::string_view name{type_name<T>()};
        addr = [&name]<std::size_t... S>(std::index_sequence<S...>, std::size_t & addr) noexcept
        {
            // thx PaperBirdMaster
            // https://stackoverflow.com/a/56326294/9807802
            return ((addr = [](std::size_t const &seed, char letter) noexcept {
                        return ((seed ^ letter) << 1);
                    }(addr, name[S])),
                    ...);
        }
        (std::make_index_sequence<name.size()>{}, addr);

        return {name, addr};
    }

    template <typename T>
    constexpr type_info type_id(T) noexcept { return type_id<T>(); }
} // namespace tnt

namespace std
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    template <>
    struct hash<tnt::type_info>
    {
        constexpr std::size_t operator()(tnt::type_info const &t) const noexcept { return t.hash(); }
    };
#endif
} // namespace std

#endif //!TNT_TYPE_INFO_MIRROR_HPP