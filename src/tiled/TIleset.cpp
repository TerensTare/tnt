// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "utils/Logger.hpp"
#include "tiled/Tileset.hpp"

tnt::tmx::Tileset::Tileset(
    std::unordered_map<std::string, std::string> &data,
    std::unordered_map<std::string, std::string> const &props,
    std::vector<tnt::tmx::Tile> const &tiles_)
    : properties{props}, tiles{tiles_},
      name{data["name"]}, source{data["source"]},
      firstGID{std::atoi(data["firstgid"].c_str())},
      imageW{std::atoi(data["width"].c_str())},
      imageH{std::atoi(data["height"].c_str())},
      tileW{std::atoi(data["tilewidth"].c_str())},
      tileH{std::atoi(data["tileheight"].c_str())},
      offsetX{std::atoi(data["tileoffsetX"].c_str())},
      offsetY{std::atoi(data["tileoffsetY"].c_str())},
      spacing{std::atoi(data["spacing"].c_str())},
      margin{std::atoi(data["margin"].c_str())},
      tileCount{std::atoi(data["tilecount"].c_str())},
      transparentColour{std::atoi(data["red"].c_str()),
                        std::atoi(data["green"].c_str()),
                        std::atoi(data["blue"].c_str())} {}

tnt::tmx::Tileset::~Tileset() noexcept
{
    properties.clear();
    std::unordered_map<std::string, std::string>{}.swap(properties);

    tiles.clear();
    std::vector<Tile>{}.swap(tiles);
}

unsigned tnt::tmx::Tileset::getFirstGID() const noexcept { return firstGID; }
unsigned tnt::tmx::Tileset::getLastGID() const noexcept { return lastGID; }

unsigned tnt::tmx::Tileset::getImageWidth() const noexcept { return imageW; }
unsigned tnt::tmx::Tileset::getImageHeight() const noexcept { return imageH; }

unsigned tnt::tmx::Tileset::getTileWidth() const noexcept { return tileW; }
unsigned tnt::tmx::Tileset::getTileHeight() const noexcept { return tileH; }

unsigned tnt::tmx::Tileset::getSpacing() const noexcept { return spacing; }
unsigned tnt::tmx::Tileset::getMargin() const noexcept { return margin; }

unsigned tnt::tmx::Tileset::getOffsetX() const noexcept { return offsetX; }
unsigned tnt::tmx::Tileset::getOffsetY() const noexcept { return offsetY; }

unsigned tnt::tmx::Tileset::getTileCount() const noexcept { return tileCount; }

std::array<unsigned, 3> tnt::tmx::Tileset::getTransparentColour() const noexcept { return transparentColour; }

std::string tnt::tmx::Tileset::getName() const noexcept { return name; }
std::string tnt::tmx::Tileset::getSource() const noexcept { return source; }

std::string tnt::tmx::Tileset::getProperty(std::string const &name) noexcept
{
    if (std::unordered_map<std::string, std::string>::const_iterator it{properties.find(name)}; it != properties.end())
        return it->second;
    else
        tnt::logger::debug("Property {} doesn't exist in Tileset {}!!", name, this->name);
}

tnt::tmx::Tile *tnt::tmx::Tileset::getTile(unsigned id)
{
    for (unsigned idx{0}; idx < lastGID; ++idx)
        if (id == tiles[id].getID())
            return &tiles[id];
    tnt::logger::debug("Tile {} doesn't exist in the Tileset {}!!", id, name);
    return nullptr;
}