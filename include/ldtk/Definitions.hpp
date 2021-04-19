#ifndef TNT_LDTK_JSON_DEFINITIONS_HPP
#define TNT_LDTK_JSON_DEFINITIONS_HPP

#include <string>
#include <vector>

#include "math/Rectangle.hpp"

// TODO:
// EntityDefinition::color and similar should be SDL_Color
// recheck data arrangement and remove unnecessary stuff
// offsets for TileInstance::px
// (maybe)
// use references on shared data ??
// replace strings with enums where possible ??

namespace tnt::ldtk
{
    /// @brief An enum that describes how levels are organized in this project (ie. linearly or in a 2D
    /// Possible values: `Free`, `GridVania`, `LinearHorizontal`, `LinearVertical`
    enum class WorldLayout
    {
        Free,
        GridVania,
        LinearHorizontal,
        LinearVertical
    };

    /// @brief Definition of a single entity
    struct EntityDefinition final
    {
        float pivotX;           ///< Pivot X coordinate, from 0 to 1.0
        float pivotY;           ///< Pivot Y coordinate, from 0 to 1.0
        std::uint32_t height;   ///< Pixel height
        std::uint32_t uid;      ///< Unique integer identifier
        std::uint32_t width;    ///< Pixel width
        std::string color;      ///< Base entity color
        std::string identifier; ///< Unique string identifier
    };

    /// @brief The definition of a single enum value
    struct EnumValueDefinition final
    {
        tnt::Rectangle tileSrcRect; ///< Rectangle referring to the tile in the tileset image
        std::string id;             ///< The value of the enum
        std::uint32_t tileId;       ///< The optional id of the tile, 0 if no id
    };

    /// @brief The definition of an enum
    struct EnumDefinition final
    {
        std::uint32_t uid;                       ///< Unique integer identifier
        std::uint32_t iconTilesetUid;            ///< Tileset uid (if given)
        std::string externalRelPath;             ///< The path to the file providing this enum
        std::string identifier;                  ///< Unique string identifier
        std::vector<EnumValueDefinition> values; ///< All possible enum values, with their optional Tile infos.
    };

    struct IntGridValueDefinition final
    {
        std::uint32_t value;    ///< The value of the IntGrid
        std::string color;      ///< The color of the grid
        std::string identifier; ///< Unique string identifier
    };

    struct LayerDefinition final
    {
        std::uint32_t autoSourceLayerDefUid;               ///< The source layer uid used by this autolayer
        std::uint32_t autoTilesetDefUid;                   ///< The tileset uid used by this autolayer
        std::uint32_t gridSize;                            ///< Width and height of the grid in pixels
        std::uint32_t pxOffsetX;                           ///< X offset of the layer, in pixels (IMPORTANT: this should be added to the `LayerInstance` optional offset.
        std::uint32_t pxOffsetY;                           ///< Y offset of the layer, in pixels (IMPORTANT: this should be added to the `LayerInstance` optional offset.
        std::uint32_t tilesetDefUid;                       ///< The tileset uid used by this tile layer.
        std::uint32_t uid;                                 ///< Unique integer identifier
        float displayOpacity;                              ///< Opacity of the layer (from 0 to 1.0)
        std::string identifier;                            ///< Unique string identifier
        std::string type;                                  ///< The type of the layer
        std::vector<IntGridValueDefinition> intGridValues; ///< An array that defines extra optional info for each IntGrid value. The array is sorted using value (ascending).
    };

    struct TilesetDefinition final
    {
        std::string identifier;     ///< Unique string identifier
        std::string relPath;        ///< Path to the source file, relative to the project's Json file
        std::uint16_t padding;      ///< Distance in pixels from image borders
        std::uint16_t pxWid;        ///< Image width in pixels
        std::uint16_t pxHei;        ///< Image height in pixels
        std::uint16_t spacing;      ///< Space in pixels between tiles
        std::uint32_t tileGridSize; ///< The size of the tiles grid in pixels
        std::uint32_t uid;          ///< Unique int identifier
    };

    struct Definitions final
    {
        std::vector<EnumDefinition> enums;         ///< All the enum definitions
        std::vector<EnumDefinition> externalEnums; ///< The definitions of external enums
        std::vector<LayerDefinition> layers;       ///< All the layers
        std::vector<EntityDefinition> entities;    ///< All the entities, including their custom fields
        std::vector<TilesetDefinition> tilesets;   ///< All the tilesets with their corresponding data
    };

    /// @brief Data related to the background of a level
    struct LevelBackgroundPosition final
    {
        tnt::Vector scale;       ///< The scale of the background image
        tnt::Vector topLeftPx;   ///< The coordinates of the top left corner of the cropped background image
        tnt::Rectangle cropRect; ///< Rectangle holding the area of the background to be shown in case it is larger than the level
    };

    /// @brief A single field
    struct FieldInstance final
    {
        std::uint32_t defUid;   ///< The id of the field definition
        std::string identifier; ///< The identifier of the field definition
        std::string type;       ///< The type of the field
        // void *value;            ///< The value of the field instance depending on @ref type
    };

    /// @brief A single tile from a given tileset
    struct TileInstance final
    {
        std::uint8_t f;  ///< The mirror transformations of the tile: 0 for none, 1 for X axis, 2 for Y axis, 3 for both
        std::uint32_t t; ///< The tile id in the corresponding tileset
        tnt::Vector src; ///< The coordinates of the tile in the tileset
        tnt::Vector px;  ///< The pixel coordinates of the tile in the layer
    };

    /// @brief Tile data in an entity instance
    struct EntityInstanceTile final
    {
        std::uint32_t tilesetUid; ///< The id of the tileset
        tnt::Rectangle srcRect;   ///< The tile in the tileset image
    };

    /// @brief A single entity
    struct EntityInstance final
    {
        tnt::Vector grid;                          ///< Grid-based coordinates
        tnt::Vector pivot;                         ///< Pivot coordinates (from 0 to 1.0)
        tnt::Vector px;                            ///< Pixel coordinates in current level coordinates space
        std::string identifier;                    ///< Entity definition identifier
        EntityInstanceTile tile;                   ///< ile used to display this entity (it could either be the default Entity tile, or some tile provided by a field value, like an Enum)
        std::uint32_t defUid;                      ///< The Entity definition UID
        std::vector<FieldInstance> fieldInstances; ///< All the custom fields
        std::uint32_t width;                       ///< The entity width in pixels
        std::uint32_t height;                      ///< The entity height in pixels
    };

    /// @brief IntGrid value instance
    struct IntGridValueInstance final
    {
        std::uint32_t coordId; ///< Coordinate ID in the layer grid
        std::uint32_t v;       ///< IntGrid value
    };

    /// @brief A single layer
    struct LayerInstance final
    {
        std::uint32_t cWid;               ///< Grid-based width
        std::uint32_t cHei;               ///< Grid-based height
        std::uint32_t gridSize;           ///< Grid size
        std::uint32_t tilesetDefUid;      ///< The definition UID of the corresponding tileset, if any
        std::uint32_t layerDefUid;        ///< The layer definition UID
        std::uint32_t levelId;            ///< Reference to the UID of the level containing this layer instance
        std::uint32_t overrideTilesetUid; ///< This layer can use another tileset by overriding the tileset UID here
        std::uint32_t seed;               ///< Random seed used for Auto-Layers rendering
        std::int32_t pxTotalOffsetX;      ///< Total layer X pixel offset, including both instance and definition offsets.
        std::int32_t pxTotalOffsetY;      ///< Total layer Y pixel offset, including both instance and definition offsets.
        std::int32_t pxOffsetX;           ///< X offset in pixels to render this layer, usually 0
        std::int32_t pxOffsetY;           ///< Y offset in pixels to render this layer, usually 0

        bool visible;               ///< Layer instance visibility
        float opacity;              ///< Layout opacity as float (from 0 to 1.0)
        std::string identifier;     ///< Layer definition identifier
        std::string tilesetRelPath; ///< The relative path to corresponding tileset, if any
        std::string type;           ///< The layer type

        std::vector<std::uint32_t> intGridCsv;       ///<  A list of all values in the IntGrid layer, stored from left to right, and top to bottom (ie. first row from left to right, followed by second row, etc). `0` means "empty cell" and IntGrid values start at 1. This array size is `__cWid` x `__cHei` cells.
        std::vector<TileInstance> autoLayerTiles;    ///< An array containing all tiles generated by Auto-layer rules. The array is already sorted in display order (ie. 1st tile is beneath 2nd, which is beneath 3rd etc.).
        std::vector<EntityInstance> entityInstances; ///< An array containing all the entity instances
        std::vector<TileInstance> gridTiles;         ///< An array containing all the grid tiles instances
    };

    /// @brief Nearby level info
    struct NeighbourLevel final
    {
        char dir;               ///< A single letter representing a direction
        std::uint32_t levelUid; ///< The level's unique integer identifier
    };

    /// @brief All the data of the level
    struct Level final
    {
        std::uint32_t pxWid;                       ///< Level width in pixels
        std::uint32_t pxHei;                       ///< Level height in pixels
        std::uint32_t worldX;                      ///< World X coordinates in pixels
        std::uint32_t worldY;                      ///< World Y coordinates in pixels
        std::uint32_t uid;                         ///< Unique integer identifier
        std::string bgColor;                       ///< The background color of the level
        std::string bgRelPath;                     ///< The relative path to the level background image, if any
        std::string externalRelPath;               ///<This value is not null if the project option "Save levels separately" is enabled. In this case, this relative path points to the level Json file
        std::string identifier;                    ///< Unique string identifier
        LevelBackgroundPosition bgPos;             ///< The data of the position of the bg image, if one
        std::vector<NeighbourLevel> neighbours;    ///< An array listing all other levels touching this one on the world map. In "linear" world layouts, this array is populated with previous/next levels in array, and `dir` depends on the linear horizontal/vertical layout
        std::vector<FieldInstance> fieldInstances; ///< An array containing this level's custom field values
        std::vector<LayerInstance> layerInstances; ///< An array containing all Layer instances.
    };

    /// @brief The "main" unit of a Ldtk project
    struct World final
    {
        std::uint32_t worldGridWidth;  ///< The width of the world grid in pixels
        std::uint32_t worldGridHeight; ///< The height of the world grid in pixels
        std::string bgColor;           ///< Project's background color
        std::string jsonVersion;       ///< File format version
        Definitions defs;              ///< All the definitions of the project
        bool externalLevels;           ///< `true` if the project stores levels in external files
        std::vector<Level> levels;     ///< All the levels of the project. The order of this array is only relevant in `LinearHorizontal` and `linearVertical` world layouts (see `worldLayout` value). Otherwise, you should refer to the `worldX`,`worldY` coordinates of each Level.
        WorldLayout worldLayout;       ///< The way levels are organised with each other on the project
    };
} // namespace tnt

#endif //!TNT_LDTK_JSON_DEFINITIONS_HPP