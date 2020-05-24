#ifndef TNT_JSON_TYPES_HPP
#define TNT_JSON_TYPES_HPP

#include <nlohmann/json.hpp>

#include "math/Rectangle.hpp"

namespace nlohmann
{
    template <>
    struct adl_serializer<tnt::Vector>
    {
        inline static void to_json(nlohmann::json &j, tnt::Vector const &v)
        {
            j = nlohmann::json{{"x", v.x}, {"y", v.y}};
        }

        inline static void from_json(nlohmann::json const &j, tnt::Vector &v)
        {
            j.at("x").get_to(v.x);
            j.at("y").get_to(v.y);
        }
    };

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

#endif //!TNT_JSON_TYPES_HPP