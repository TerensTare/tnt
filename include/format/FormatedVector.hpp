#ifndef TNT_FMT_FORMATED_VECTOR_HPP
#define TNT_FMT_FORMATED_VECTOR_HPP

#include <fmt/format-inl.h>
#include "math/Vector.hpp"

namespace fmt
{
    template <>
    struct formatter<tnt::Vector>
    {
        char presentation{' '};

        constexpr auto parse(format_parse_context &ctx)
        {
            char const *it{ctx.begin()};
            char const *end{ctx.end()};
            if (it != end && *it == ' ')
                presentation = *it++;

            if (it != end && *it != '}')
                throw fmt::format_error{"invalid format"};
            return it;
        }

        template <typename FormatContext>
        inline auto format(tnt::Vector const &vec, FormatContext &ctx)
        {
            return format_to(ctx.out(), "({}, {})", vec.x, vec.y);
        }
    };
} // namespace fmt

#endif //!TNT_FMT_FORMATED_VECTOR_HPP