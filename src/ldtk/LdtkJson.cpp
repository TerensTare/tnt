// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <nlohmann/json.hpp>

#include "json/JsonRectangle.hpp"
#include "json/JsonVector.hpp"

#include "ldtk/LdtkJson.hpp"

template <std::default_initializable T>
inline static auto get_optional(nlohmann::json const &j, const char *field)
{
    if (j.contains(field))
        return j[field].get<T>();
    return T{};
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::EntityDefinition &def)
{
    def.pivotX = j["pivotX"].get<float>();
    def.pivotY = j["pivotY"].get<float>();
    def.height = j["height"].get<std::uint32_t>();
    def.uid = j["uid"].get<std::uint32_t>();
    def.width = j["width"].get<std::uint32_t>();
    def.color = j["color"].get<std::string>();
    def.identifier = j["identifier"].get<std::string>();
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::EnumValueDefinition &def)
{
    def.tileSrcRect = j["__tileSrcRect"].get<tnt::Rectangle>();
    def.id = j["id"].get<std::string>();
    def.tileId = get_optional<std::uint32_t>(j, "tileId");
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::EnumDefinition &def)
{
    def.externalRelPath = get_optional<std::string>(j, "externalRelPath");
    def.iconTilesetUid = get_optional<std::uint32_t>(j, "iconTilesetUid");
    def.identifier = j["identifier"].get<std::string>();
    def.uid = j["uid"].get<std::uint32_t>();
    def.values = j["values"].get<std::vector<tnt::ldtk::EnumValueDefinition>>();
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::IntGridValueDefinition &def)
{
    def.color = j["color"].get<std::string>();
    def.identifier = get_optional<std::string>(j, "identifier");
    def.value = j["value"].get<std::uint32_t>();
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::LayerDefinition &def)
{
    def.type = j["__type"].get<std::string>();
    def.autoSourceLayerDefUid = get_optional<std::uint32_t>(j, "autoSourceLayerDefUid");
    def.autoTilesetDefUid = get_optional<std::uint32_t>(j, "autoTilesetDefUid");
    def.displayOpacity = j["displayOpacity"].get<float>();
    def.gridSize = j["gridSize"].get<std::uint32_t>();
    def.identifier = j["identifier"].get<std::string>();
    def.intGridValues = j["intGridValues"].get<std::vector<tnt::ldtk::IntGridValueDefinition>>();
    def.pxOffsetX = j["pxOffsetX"].get<std::uint32_t>();
    def.pxOffsetY = j["pxOffsetY"].get<std::uint32_t>();
    def.tilesetDefUid = get_optional<std::uint32_t>(j, "tilesetDefUid");
    def.uid = j["uid"].get<std::uint32_t>();
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::TilesetDefinition &def)
{
    def.identifier = j["identifier"].get<std::string>();
    def.padding = j["padding"].get<std::uint16_t>();
    def.pxWid = j["pxHei"].get<std::uint16_t>();
    def.pxHei = j["pxHei"].get<std::uint16_t>();
    def.relPath = j["relPath"].get<std::string>();
    def.spacing = j["spacing"].get<std::uint16_t>();
    def.tileGridSize = j["tileGridSize"].get<std::uint32_t>();
    def.uid = j["uid"].get<std::uint32_t>();
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::Definitions &def)
{
    def.entities = j["entities"].get<std::vector<tnt::ldtk::EntityDefinition>>();
    def.enums = j["enums"].get<std::vector<tnt::ldtk::EnumDefinition>>();
    def.externalEnums = j["externalEnums"].get<std::vector<tnt::ldtk::EnumDefinition>>();
    def.layers = j["layers"].get<std::vector<tnt::ldtk::LayerDefinition>>();
    def.tilesets = j["tilesets"].get<std::vector<tnt::ldtk::TilesetDefinition>>();
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::LevelBackgroundPosition &def)
{
    def.cropRect = j["cropRect"].get<tnt::Rectangle>();
    def.scale = j["scale"].get<tnt::Vector>();
    def.topLeftPx = j["topLeftPx"].get<tnt::Vector>();
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::FieldInstance &def)
{
    def.defUid = j["defUid"].get<std::uint32_t>();
    def.identifier = j["__identifier"].get<std::string>();
    def.type = j["__type"].get<std::string>();
    // def.value = (void *)j["__value"];
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::TileInstance &def)
{
    def.f = j["f"].get<std::uint8_t>();
    def.px = j["px"].get<tnt::Vector>();
    def.src = j["src"].get<tnt::Vector>();
    def.t = j["t"].get<std::uint32_t>();
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::EntityInstanceTile &def)
{
    def.srcRect = j["srcRect"].get<tnt::Rectangle>();
    def.tilesetUid = j["tilesetUid"].get<std::uint32_t>();
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::EntityInstance &def)
{
    def.grid = j["__grid"].get<tnt::Vector>();
    def.identifier = j["__identifier"].get<std::string>();
    def.pivot = j["__pivot"].get<tnt::Vector>();
    def.tile = get_optional<tnt::ldtk::EntityInstanceTile>(j, "__tile");
    def.defUid = j["defUid"].get<std::uint32_t>();
    def.fieldInstances = j["fieldInstances"].get<std::vector<tnt::ldtk::FieldInstance>>();
    def.width = j["width"].get<std::uint32_t>();
    def.height = j["height"].get<std::uint32_t>();
    def.px = j["px"].get<tnt::Vector>();
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::IntGridValueInstance &def)
{
    def.coordId = j["coordId"].get<std::uint32_t>();
    def.v = j["v"].get<std::uint32_t>();
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::LayerInstance &def)
{
    def.cWid = j["__cWid"].get<std::uint32_t>();
    def.cHei = j["__cHei"].get<std::uint32_t>();
    def.gridSize = j["__gridSize"].get<std::uint32_t>();
    def.identifier = j["__identifier"].get<std::string>();
    def.opacity = j["__opacity"].get<float>();
    def.pxTotalOffsetX = j["__pxTotalOffsetX"].get<std::int32_t>();
    def.pxTotalOffsetY = j["__pxTotalOffsetY"].get<std::int32_t>();
    def.tilesetDefUid = get_optional<std::uint32_t>(j, "__tilesetDefUid");
    def.tilesetRelPath = get_optional<std::string>(j, "__tilesetRelPath");
    def.type = j["__type"].get<std::string>();
    def.autoLayerTiles = j["autoLayerTiles"].get<std::vector<tnt::ldtk::TileInstance>>();
    def.entityInstances = j["entityInstances"].get<std::vector<tnt::ldtk::EntityInstance>>();
    def.gridTiles = j["gridTiles"].get<std::vector<tnt::ldtk::TileInstance>>();
    def.intGridCsv = j["intGridCsv"].get<std::vector<std::uint32_t>>();
    def.layerDefUid = j["layerDefUid"].get<std::uint32_t>();
    def.levelId = j["levelId"].get<std::uint32_t>();
    def.overrideTilesetUid = j["overrideTilesetUid"].get<std::uint32_t>();
    def.pxOffsetX = j["pxOffsetX"].get<std::int32_t>();
    def.pxOffsetY = j["pxOffsetY"].get<std::int32_t>();
    def.visible = j["visible"].get<bool>();
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::NeighbourLevel &def)
{
    def.dir = j["dir"].get<char>();
    def.levelUid = j["levelUid"].get<std::uint32_t>();
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::Level &def)
{
    def.bgColor = j["__bgColor"].get<std::string>();
    def.bgPos = get_optional<tnt::ldtk::LevelBackgroundPosition>(j, "__bgPos");
    def.neighbours = j["__neighbours"].get<std::vector<tnt::ldtk::NeighbourLevel>>();
    def.bgRelPath = get_optional<std::string>(j, "bgRelPath");
    def.externalRelPath = get_optional<std::string>(j, "externalRelPath");
    def.fieldInstances = j["fieldInstances"].get<std::vector<tnt::ldtk::FieldInstance>>();
    def.identifier = j["identifier"].get<std::string>();
    def.layerInstances = get_optional<std::vector<tnt::ldtk::LayerInstance>>(j, "layerInstances");
    def.pxWid = j["pxWid"].get<std::uint32_t>();
    def.pxHei = j["pxHei"].get<std::uint32_t>();
    def.uid = j["uid"].get<std::uint32_t>();
    def.worldX = j["worldX"].get<std::uint32_t>();
    def.worldY = j["worldY"].get<std::uint32_t>();
}

void tnt::ldtk::from_json(nlohmann::json const &j, tnt::ldtk::World &def)
{
    def.bgColor = j["bgColor"].get<std::string>();
    def.defs = j["defs"].get<tnt::ldtk::Definitions>();
    def.externalLevels = j["externalLevels"].get<bool>();
    def.jsonVersion = j["jsonVersion"].get<std::string>();
    def.levels = j["levels"].get<std::vector<tnt::ldtk::Level>>();
    def.worldGridWidth = j["worldGridWidth"].get<std::uint32_t>();
    def.worldGridHeight = j["worldGridHeight"].get<std::uint32_t>();
    def.worldLayout = j["worldLayout"].get<tnt::ldtk::WorldLayout>();
}