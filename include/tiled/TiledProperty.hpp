#ifndef TNT_TILED_PROPERTY_HPP
#define TNT_TILED_PROPERTY_HPP

#include <string>

// TODO(maybe):
// represent a property as a std::map ?? BUT without type.

namespace tnt::tmx
{
    struct Property final
    {
        inline Property(std::string_view name_,
                        std::string_view type_,
                        std::string_view value_) noexcept
            : name{name_}, type{type_}, value{value_} {}

        std::string name;
        std::string type;
        std::string value;
    };
} // namespace tnt::tmx

#endif //!TNT_TILED_PROPERTY_HPP