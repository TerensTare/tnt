#ifndef TNT_LDTK_JSON_HPP
#define TNT_LDTK_JSON_HPP

#include <nlohmann/json_fwd.hpp>

#include "core/Config.hpp"
#include "types/EnumTraits.hpp"
#include "ldtk/Definitions.hpp"

// TODO(maybe):
// a Predef/Forward file for only the forward declaration of the types (until modules) ??

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace tnt::ldtk
{
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::EntityDefinition &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::EnumValueDefinition &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::EnumDefinition &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::IntGridValueDefinition &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::LayerDefinition &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::TilesetDefinition &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::Definitions &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::LevelBackgroundPosition &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::FieldInstance &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::TileInstance &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::EntityInstanceTile &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::EntityInstance &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::IntGridValueInstance &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::LayerInstance &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::NeighbourLevel &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::Level &x);
    TNT_API void from_json(nlohmann::json const &j, tnt::ldtk::World &x);
}

namespace tnt
{
    template <>
    struct enum_traits<tnt::ldtk::WorldLayout> final
    {
        using self = tnt::ldtk::WorldLayout;
        using base = std::underlying_type_t<tnt::ldtk::WorldLayout>;

        inline static constexpr self count = static_cast<self>(static_cast<base>(self::LinearVertical) + 1);
        inline static constexpr bool jsonable = true;
    };
}
#endif //!DOXYGEN_SHOULD_SKIP_THIS

#endif //!TNT_LDTK_JSON_HPP