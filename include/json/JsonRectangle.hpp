#ifndef TNT_JSON_SERIALIZE_RECTANGLE_HPP
#define TNT_JSON_SERIALIZE_RECTANGLE_HPP

#include <nlohmann/json_fwd.hpp>

#include "core/Config.hpp"
#include "math/Rectangle.hpp"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace tnt
{
    TNT_API void to_json(nlohmann::json &j, tnt::Rectangle const &rect) noexcept;
    TNT_API void from_json(nlohmann::json const &j, tnt::Rectangle &rect) noexcept;
} // namespace nlohmann
#endif

#endif //!TNT_JSON_SERIALIZE_RECTANGLE_HPP