#ifndef TNT_FMT_FORMATTED_HASH_STRING_HPP
#define TNT_FMT_FORMATTED_HASH_STRING_HPP

#include <fmt/format.h>
#include "types/HashedString.hpp"

template <tnt::detail::character Char>
struct fmt::formatter<tnt::basic_hashed_string<Char>>
    : fmt::formatter<std::basic_string_view<Char>>
{
    template <typename FormatContext>
    inline auto format(tnt::basic_hashed_string<Char> const &str, FormatContext &ctx)
    {
        return fmt::formatter<std::basic_string_view<Char>>::format(str.data(), ctx);
    }
};

#endif //!TNT_FMT_FORMATTED_HASH_STRING_HPP