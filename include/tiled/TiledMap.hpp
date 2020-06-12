#ifndef TNT_TILED_MAP_HPP
#define TNT_TILED_MAP_HPP

#include <string_view>
#include <SDL2/SDL_pixels.h>
#include "tiled/TiledLayer.hpp"

namespace tnt::tmx
{
    struct Map final
    {
        bool infinite;

        int height;
        int hexindexlength; // hexagonal-only
        int nextlayerid;
        int tileheight;

        SDL_Color backgroundcolor;

        std::string orientation;
        std::string renderorder{"right-down"};
        std::string staggeraxis;  // hexagonal-only
        std::string staggerindex; // hexagonal-only

        std::vector<tmx::Property> properties;
        std::vector<tmx::Layer> layers;
    };
} // namespace tnt::tmx

#endif //!TNT_TILED_MAP_HPP