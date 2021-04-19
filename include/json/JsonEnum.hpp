#ifndef TNT_AUTOMATIC_ENUM_TO_JSON_HPP
#define TNT_AUTOMATIC_ENUM_TO_JSON_HPP

#include <nlohmann/json.hpp>
#include "mirror/Light.hpp"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// clang-format off
template <typename E>
    requires (std::is_enum_v<E>
        and requires { { tnt::enum_traits<E>::jsonable } -> std::same_as<bool const>; }
        and (tnt::enum_traits<E>::jsonable == true))
struct nlohmann::adl_serializer<E>
{
    // clang-format on

    static void from_json(nlohmann::json const &j, E &e) noexcept
    {
        e = tnt::enum_value<E>(j.get<std::string_view>());
    }

    static void to_json(nlohmann::json &j, E const &e) noexcept
    {
        j = tnt::enum_name(e);
    }
};
#endif

#endif //!TNT_AUTOMATIC_ENUM_TO_JSON_HPP