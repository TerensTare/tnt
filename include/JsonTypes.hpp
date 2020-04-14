#ifndef JSON_TYPES_HPP
#define JSON_TYPES_HPP

#include <nlohmann/json.hpp>

#include "Vector.hpp"

namespace nlohmann
{
    template <> struct adl_serializer<tnt::Vector>
    {
        static void to_json(nlohmann::json &j, const tnt::Vector &v)
        {
            j = nlohmann::json{{"x", v.x}, {"y", v.y}};
        }

        void from_json(const nlohmann::json &j, tnt::Vector &v)
        {
            j.at("x").get_to(v.x);
            j.at("y").get_to(v.y);
        }
    };
} // namespace nlohmann

#endif //! JSON_TYPES_HPP