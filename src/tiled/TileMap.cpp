// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "tiled/TileMap.hpp"
#include "utils/Logger.hpp"

tnt::tmx::TileMap::TileMap() {}

tnt::tmx::TileMap::~TileMap() noexcept
{
    properties.clear();
    std::unordered_map<std::string, std::string>{}.swap(properties);

    tilesets.clear();
    std::vector<Tileset>{}.swap(tilesets);

    layers.clear();
    std::vector<TileLayer>{}.swap(layers);
}

void tnt::tmx::TileMap::setMapSettings(std::vector<std::string> const &mapData, std::unordered_map<std::string, std::string> const &mapProps) noexcept
{
    version = std::stoi(mapData[0]);
    orientation = mapData[1];
    renderOrder = mapData[2];
    width = std::stoi(mapData[3]);
    height = std::stoi(mapData[4]);
    tileW = std::stoi(mapData[5]);
    tileH = std::stoi(mapData[6]);

    bgColor[0] = std::stoi(mapData[8]);
    bgColor[1] = std::stoi(mapData[8]);
    bgColor[2] = std::stoi(mapData[8]);

    properties = mapProps;
}

float tnt::tmx::TileMap::getVersion() const noexcept { return version; }

unsigned tnt::tmx::TileMap::getWidth() const noexcept { return width; }
unsigned tnt::tmx::TileMap::getHeight() const noexcept { return height; }
unsigned tnt::tmx::TileMap::getTileWidth() const noexcept { return tileW; }
unsigned tnt::tmx::TileMap::getTileHeight() const noexcept { return tileH; }

std::array<unsigned, 3> tnt::tmx::TileMap::getBackgroundColor() const noexcept { return bgColor; }

std::string tnt::tmx::TileMap::getOrientation() const noexcept { return orientation; }
std::string tnt::tmx::TileMap::getRenderOrder() const noexcept { return renderOrder; }

void tnt::tmx::TileMap::addTileset(tnt::tmx::Tileset const &ts) noexcept { tilesets.push_back(ts); }

tnt::tmx::Tileset *tnt::tmx::TileMap::getTileset(std::string const &name) noexcept
{
    for (unsigned idx{0}; idx < tilesets.size(); ++idx)
        if (tilesets[idx].getName() == name)
            return &tilesets[idx];
    tnt::logger::debug("Tileset {} doesn't exist on this TileMap!!", name);
    return nullptr;
}

void tnt::tmx::TileMap::addLayer(tnt::tmx::TileLayer const &name) noexcept { layers.push_back(name); }

tnt::tmx::TileLayer *tnt::tmx::TileMap::getLayer(std::string const &name) noexcept
{
    for (unsigned idx{0}; idx < layers.size(); ++idx)
        if (layers[idx].getName() == name)
            return &layers[idx];
    tnt::logger::debug("TileLayer {} doesn't exist on this TileMap!!", name);
    return nullptr;
}