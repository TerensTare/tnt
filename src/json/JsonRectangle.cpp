// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <nlohmann/json.hpp>
#include "json/JsonRectangle.hpp"

void tnt::to_json(nlohmann::json &j, tnt::Rectangle const &rect) noexcept
{
    j = nlohmann::json{{"x", rect.x}, {"y", rect.y}, {"w", rect.w}, {"h", rect.h}};
}

void tnt::from_json(nlohmann::json const &j, tnt::Rectangle &rect) noexcept
{
    j.at("x").get_to(rect.x);
    j.at("y").get_to(rect.y);
    j.at("w").get_to(rect.w);
    j.at("h").get_to(rect.h);
}