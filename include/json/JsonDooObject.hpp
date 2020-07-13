#ifndef TNT_JSON_SERLIAZE_DOO_OBJECT_HPP
#define TNT_JSON_SERLIAZE_DOO_OBJECT_HPP

#include <nlohmann/json.hpp>

#include "doo_ecs/Base.hpp"
#include "json/JsonVector.hpp"

namespace nlohmann
{
    template <>
    struct adl_serializer<tnt::doo::object_data>
    {
        inline static void from_json(json const &j, tnt::doo::object_data &data_)
        {
            j.at("angle").get_to(data_.angle);
            j.at("scale").get_to(data_.scale);
            j.at("pos").get_to(data_.pos);
        }

        inline static void to_json(json &j, tnt::doo::object_data const &data_)
        {
            j = json{{"angle", data_.angle}, {"scale", data_.scale}, {"pos", data_.pos}};
        }
    };
} // namespace nlohmann

#endif //!TNT_JSON_SERLIAZE_DOO_OBJECT_HPP