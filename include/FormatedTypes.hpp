#ifndef TNT_FMT_TYPES_HPP
#define TNT_FMT_TYPES_HPP

#include <fmt/format-inl.h>
#include "math/Rectangle.hpp"

namespace fmt
{
    template <>
    struct formatter<tnt::Vector>
    {
        char presentation{'f'};

        inline constexpr auto parse(format_parse_context &ctx)
        {
            char const *it{ctx.begin()};
            char const *end{ctx.end()};
            if (it != end && *it == 'f')
                presentation = *it++;

            if (it != end && *it != '}')
                throw fmt::format_error{"invalid format"};
            return it;
        }

        template <typename FormatContext>
        inline auto format(tnt::Vector const &vec, FormatContext &ctx)
        {
            return format_to(ctx.out(), "({:.1f}, {:.1f})", vec.x, vec.y);
        }
    };

    template <>
    struct formatter<tnt::Rectangle>
    {
        char presentation{'f'};

        inline constexpr auto parse(format_parse_context &ctx)
        {
            char const *it{ctx.begin()};
            char const *end{ctx.end()};
            if (it != end && *it == 'f')
                presentation = *it++;

            if (it != end && *it != '}')
                throw fmt::format_error{"invalid format"};
            return it;
        }

        template <typename FormatContext>
        inline auto format(tnt::Rectangle const &rect, FormatContext &ctx)
        {
            return format_to(ctx.out(),
                             "({:.1f}, {:.1f}, {:.1f}, {:.1f})",
                             rect.x, rect.y, rect.w, rect.h);
        }
    };
} // namespace fmt

#endif //!TNT_FMT_TYPES_HPP