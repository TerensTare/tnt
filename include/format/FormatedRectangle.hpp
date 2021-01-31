#ifndef TNT_FMT_FORMATED_RECTANGLE_HPP
#define TNT_FMT_FORMATED_RECTANGLE_HPP

#include <fmt/format-inl.h>
#include "math/Rectangle.hpp"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace fmt
{
    template <>
    struct formatter<tnt::Rectangle>
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
        inline auto format(tnt::Rectangle const &rect, FormatContext &ctx)
        {
            return format_to(ctx.out(), "({}, {}, {}, {})",
                             rect.x, rect.y, rect.w, rect.h);
        }
    };
} // namespace fmt
#endif

#endif //!TNT_FMT_FORMATED_RECTANGLE_HPP