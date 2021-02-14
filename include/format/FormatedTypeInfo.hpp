#ifndef TNT_FMT_FORMATTED_TYPE_INFO_HPP
#define TNT_FMT_FORMATTED_TYPE_INFO_HPP

#include <fmt/format.h>
#include "mirror/TypeInfo.hpp"

template <>
struct fmt::formatter<tnt::type_info>
    : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    inline auto format(tnt::type_info const &info, FormatContext &ctx)
    {
        return fmt::formatter<std::string_view>::format(info.name(), ctx);
    }
};

#endif //!TNT_FMT_FORMATTED_TYPE_INFO_HPP