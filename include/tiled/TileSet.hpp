#ifndef TNT_TILED_TILESET_HPP
#define TNT_TILED_TILESET_HPP

#include <span>
#include <vector>

#include "tiled/TileGrid.hpp"
#include "tiled/Tile.hpp"

// TODO(maybe):
// terrain support ??

namespace tnt::tmx
{
    struct Tileset final
    {
        inline Tileset(std::string_view image_, int cols, int firstGID, int imageW, int imageH,
                       int margin_, int spacing_, int tileCnt, int tileW, int tileH,
                       int tileoffsetX = 0, int tileoffsetY = 0) noexcept {}

        int columns;
        int firstgid;
        int imagewidth;
        int imageheight;
        int margin;
        int spacing;
        int tilecount;
        int tilewidth;
        int tileheight;

        int tileoffsetx; // optional
        int tileoffsety; // optional

        std::string backgroundcolor;  // (optional) #RRGGBBAA
        std::string transparentcolor; // (optional) #RRGGBBAA
        std::string image;
        std::string name;
        std::string source;

        tmx::Grid grid;

        std::vector<tmx::Property> properties;
        std::vector<tmx::Tile> tiles; // (optional)
    };
} // namespace tnt::tmx

#endif //!TNT_TILED_TILESET_HPP