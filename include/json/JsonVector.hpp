#ifndef TNT_JSON_SERIALIZE_VECTOR_HPP
#define TNT_JSON_SERIALIZE_VECTOR_HPP

#include <nlohmann/json_fwd.hpp>

#include "core/Config.hpp"
#include "math/Vector.hpp"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace tnt
{
    TNT_API void to_json(nlohmann::json &j, tnt::Vector const &vec) noexcept;
    TNT_API void from_json(nlohmann::json const &j, tnt::Vector &vec) noexcept;
} // namespace nlohmann
#endif

#endif //!TNT_JSON_SERIALIZE_VECTOR_HPP