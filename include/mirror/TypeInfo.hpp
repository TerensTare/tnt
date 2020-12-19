#ifndef TNT_TYPE_INFO_MIRROR_HPP
#define TNT_TYPE_INFO_MIRROR_HPP

#include <functional>

namespace tnt
{
    namespace detail
    {
        template <typename T>
        using unwrapped = std::remove_cvref_t<std::decay_t<std::remove_all_extents_t<T>>>;
    } // namespace detail

    class [[nodiscard]] type_info final
    {
    public:
        constexpr type_info(type_info const &other) noexcept
            : name_{other.name_}, name_s{other.name_s}, index{other.index} {}

        constexpr type_info(type_info && other) noexcept
            : name_{std::exchange(other.name_, {})},
              name_s{std::exchange(other.name_s, {})},
              index{std::exchange(other.index, {})} {}

        constexpr type_info &operator=(type_info const &other) noexcept
        {
            name_ = other.name_;
            name_s = other.name_s;
            index = other.index;
            return *this;
        }

        constexpr type_info &operator=(type_info &&other) noexcept
        {
            name_ = std::exchange(other.name_, {});
            name_s = std::exchange(other.name_s, {});
            index = std::exchange(other.index, {});
            return *this;
        }

        // TODO: move ops

        constexpr std::size_t hash() const noexcept { return index; }

        constexpr const char *name() const noexcept { return name_; }

        constexpr bool operator==(type_info const &other) const noexcept
        {
            return index == other.index;
        }

    private:
        constexpr type_info(std::string_view n, std::size_t const &h) noexcept
            : name_{n.data()}, name_s{n.size()}, index{h} {}

        const char *name_;
        std::size_t name_s;

        std::size_t index;

        template <typename T>
        friend constexpr type_info type_id() noexcept;
    };

    template <typename T>
    constexpr type_info type_id() noexcept
    {
        if constexpr (not std::same_as<T, detail::unwrapped<T>>)
            return type_id<detail::unwrapped<T>>();

            // thx Boost.UT authors
            // https://github.com/boost-ext/ut/blob/3b05dca6a629497910cf8e92aebcaead0124c8b4/include/boost/ut.hpp#L228
#if defined(_MSC_VER) && !defined(__clang__)
        constexpr std::string_view name{__FUNCSIG__};
#elif defined(__clang__)
        constexpr std::string_view name{&__PRETTY_FUNCTION__[35], sizeof(__PRETTY_FUNCTION__) - 37};
#else
        constexpr std::string_view name{&__PRETTY_FUNCTION__[50], sizeof(__PRETTY_FUNCTION__) - 52};
#endif

        std::size_t addr{};
        addr = [&name, addr ]<std::size_t... S>(std::index_sequence<S...>) mutable noexcept
        {
            // thx PaperBirdMaster
            // https://stackoverflow.com/a/56326294/9807802
            constexpr auto hash = [](std::size_t const &seed, char letter) noexcept {
                return ((seed ^ letter) << 1);
            };

            return ((addr = hash(addr, name[S])), ...);
        }
        (std::make_index_sequence<name.size()>{});

        return type_info{name, addr};
    }

    template <typename T>
    constexpr type_info type_id(T) noexcept { return type_id<T>(); }
} // namespace tnt

namespace std
{
    template <>
    struct hash<tnt::type_info>
    {
        constexpr std::size_t operator()(tnt::type_info const &t) const noexcept { return t.hash(); }
    };
} // namespace std

#endif //!TNT_TYPE_INFO_MIRROR_HPP