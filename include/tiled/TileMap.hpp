#ifndef TNT_TMX_TILE_MAP_HPP
#define TNT_TMX_TILE_MAP_HPP

#include "tiled/Tileset.hpp"
#include "tiled/TileLayer.hpp"

namespace tnt::tmx
{
class TileMap final
{
public:
    TileMap();
    TileMap(TileMap const &) = delete;
    TileMap &operator=(TileMap const &) = delete;

    ~TileMap() noexcept;

    void setMapSettings(
        std::vector<std::string> const &mapData,
        std::unordered_map<std::string, std::string> const &mapProps) noexcept;

    float getVersion() const noexcept;
    unsigned getWidth() const noexcept;
    unsigned getHeight() const noexcept;
    unsigned getTileWidth() const noexcept;
    unsigned getTileHeight() const noexcept;
    std::array<unsigned, 3> getBackgroundColor() const noexcept;
    std::string getOrientation() const noexcept;
    std::string getRenderOrder() const noexcept;

    void addTileset(Tileset const &ts) noexcept;
    Tileset *getTileset(std::string const &name) noexcept;

    void addLayer(TileLayer const &layer) noexcept;
    TileLayer *getLayer(std::string const &name) noexcept;

private:
    float version;
    unsigned width, height;
    unsigned tileW, tileH;
    std::array<unsigned, 3> bgColor;
    std::string orientation;
    std::string renderOrder;
    std::unordered_map<std::string, std::string> properties;
    std::vector<TileLayer> layers;
    std::vector<Tileset> tilesets;
};
} // namespace tnt::tmx

#endif //!TNT_TMX_TILE_MAP_HPP