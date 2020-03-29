// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "tiled/TileLayer.hpp"
#include "utils/Logger.hpp"

tnt::tmx::TileLayer::TileLayer(
    std::string const &layerName,
    unsigned int width, unsigned int height,
    std::unordered_map<std::string, std::string> layerProps,
    std::vector<std::vector<unsigned>> tilesVector)
    : name{layerName}, w{width}, h{height},
      properties{layerProps}, tiles{tilesVector} {}

tnt::tmx::TileLayer::~TileLayer() noexcept
{
    tiles.clear();
    std::vector<std::vector<unsigned>>{}.swap(tiles);

    properties.clear();
    std::unordered_map<std::string, std::string>{}.swap(properties);
}

unsigned tnt::tmx::TileLayer::getWidth() const noexcept { return w; }
unsigned tnt::tmx::TileLayer::getHeight() const noexcept { return h; }

std::string tnt::tmx::TileLayer::getName() const noexcept { return name; }
std::vector<std::vector<unsigned>> tnt::tmx::TileLayer::getTiles() const noexcept { return tiles; }

std::string tnt::tmx::TileLayer::getProperty(std::string const &prop) noexcept
{
    if (std::unordered_map<std::string, std::string>::const_iterator
            it{properties.find(prop)};
        it != properties.end())
        return it->second;
    tnt::logger::debug("Property {} doesn't exist in TileLayer {}!!", prop, name);
    return nullptr;
}