#ifndef TNT_HASHED_STRING_HPP
#define TNT_HASHED_STRING_HPP

#include <string_view>
#include <utility>
#include "core/Config.hpp"

// TODO:
// constexpr on std::basic_string ctor
// support for "subbed" strings by using a size_ member (coming from std::string::substr etc.)

// TODO(maybe):
// structured bindings support ??

namespace tnt
{
    namespace detail
    {
        template <typename T>
        concept character = requires
        {
            typename std::char_traits<T>::char_type;
        }
        &&(std::is_same_v<T, typename std::char_traits<T>::char_type> == true);
    } // namespace detail

    // thx Jesse C. Slicer
    // https://codereview.stackexchange.com/q/39515/228121
    template <detail::character Char>
    constexpr std::uint64_t fnv1a(std::basic_string_view<Char> str) noexcept
    {
        constexpr std::uint64_t prime{1099511628211u};
        std::uint64_t hash{14695981039346656037u};
        for (std::size_t i{}, s{str.size()}; i < s; ++i)
            hash = (hash ^ str[i]) * prime;
        return hash;
    }

    template <detail::character Char>
    class TNT_API basic_hashed_string final
    {
    public:
        using char_type = Char;
        using hash_type = std::uint64_t;

        explicit constexpr basic_hashed_string(const char_type *str_) noexcept
            : str{str_}, hash_{fnv1a<char_type>(str_)} {}

        template <typename Alloc>
        /*constexpr*/ basic_hashed_string(std::basic_string<char_type, Alloc> const &str_) noexcept
            : str{str_.data()}, hash_{fnv1a<char_type>(str_)}
        {
        }

        constexpr basic_hashed_string(basic_hashed_string const &rhs) noexcept
            : basic_hashed_string{rhs.str} {}

        constexpr basic_hashed_string(basic_hashed_string &&rhs) noexcept
            : str{std::exchange(rhs.str, nullptr)}, hash_{std::exchange(rhs.hash_, {})} {}

        constexpr bool operator==(basic_hashed_string const &rhs) const noexcept
        {
            return hash_ == rhs.hash_;
        }

        constexpr const char_type *data() const noexcept { return str; }
        constexpr std::uint64_t hash() const noexcept { return hash_; }

        constexpr operator const char_type *() const noexcept { return str; }
        constexpr operator hash_type() const noexcept { return hash_; }

    private:
        const char_type *str;
        std::uint64_t hash_;
    };

    using hashed_string = basic_hashed_string<char>;

    namespace literals
    {
        constexpr hashed_string operator""_hs(const char *ptr, std::size_t) noexcept
        {
            return hashed_string{ptr};
        }
    } // namespace literals
} // namespace tnt

#endif //!TNT_HASHED_STRING_HPP