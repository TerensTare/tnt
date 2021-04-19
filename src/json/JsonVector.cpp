// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <nlohmann/json.hpp>
#include "json/JsonVector.hpp"

void tnt::to_json(nlohmann::json &j, tnt::Vector const &vec) noexcept
{
    j = nlohmann::json{{"x", vec.x}, {"y", vec.y}};
}

void tnt::from_json(nlohmann::json const &j, tnt::Vector &vec) noexcept
{
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
}