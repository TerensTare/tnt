#ifndef TNT_TMX_TILESET_HPP
#define TNT_TMX_TILESET_HPP

#include <array>
#include <vector>
#include "tiled/Tile.hpp"

namespace tnt::tmx
{
class Tileset final
{
public:
    Tileset(
        std::unordered_map<std::string, std::string> &data,
        std::unordered_map<std::string, std::string> const &props,
        std::vector<Tile> const &tiles_);
    ~Tileset() noexcept;

    unsigned getFirstGID() const noexcept;
    unsigned getLastGID() const noexcept;
    unsigned getImageWidth() const noexcept;
    unsigned getImageHeight() const noexcept;
    unsigned getTileWidth() const noexcept;
    unsigned getTileHeight() const noexcept;
    unsigned getSpacing() const noexcept;
    unsigned getMargin() const noexcept;
    unsigned getOffsetX() const noexcept;
    unsigned getOffsetY() const noexcept;
    unsigned getTileCount() const noexcept;
    std::array<unsigned, 3> getTransparentColour() const noexcept; // (maybe) return a SDL_Color ??
    std::string getName() const noexcept;
    std::string getSource() const noexcept;
    std::string getProperty(std::string const &name) noexcept;
    Tile *getTile(unsigned id) noexcept;

private:
    unsigned firstGID, lastGID;
    unsigned imageW, imageH;
    unsigned tileW, tileH;
    unsigned spacing;
    unsigned margin;
    unsigned offsetX;
    unsigned offsetY;
    unsigned tileCount;
    std::array<unsigned, 3> transparentColour;
    std::string name;
    std::string source;
    std::unordered_map<std::string, std::string> properties;
    std::vector<Tile> tiles;
};
} // namespace tnt::tmx

#endif //!TNT_TMX_TILESET_HPP