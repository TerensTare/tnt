#ifndef TNT_JSON_SERIALIZE_RECTANGLE_HPP
#define TNT_JSON_SERIALIZE_RECTANGLE_HPP

#include <nlohmann/json.hpp>
#include "math/Rectangle.hpp"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace nlohmann
{
    template <>
    struct adl_serializer<tnt::Rectangle>
    {
        inline static void to_json(nlohmann::json &j, tnt::Rectangle const &rect)
        {
            j = nlohmann::json{{"x", rect.x}, {"y", rect.y}, {"w", rect.w}, {"h", rect.h}};
        }

        inline static void from_json(nlohmann::json const &j, tnt::Rectangle &rect)
        {
            j.at("x").get_to(rect.x);
            j.at("y").get_to(rect.y);
            j.at("w").get_to(rect.w);
            j.at("h").get_to(rect.h);
        }
    };
} // namespace nlohmann
#endif

#endif //!TNT_JSON_SERIALIZE_RECTANGLE_HPP