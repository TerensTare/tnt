#ifndef TNT_JSON_SERIALIZE_VECTOR_HPP
#define TNT_JSON_SERIALIZE_VECTOR_HPP

#include <nlohmann/json.hpp>
#include "math/Vector.hpp"

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
} // namespace nlohmann

#endif //!TNT_JSON_SERIALIZE_VECTOR_HPP