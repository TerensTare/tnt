#ifndef TNT_DOO_TILED_MAPS_HPP
#define TNT_DOO_TILED_MAPS_HPP

#include <map>
#include <string>
#include <vector>

#include <SDL2/SDL_pixels.h>

#include "math/Vector.hpp"

namespace tnt::tmx
{
    struct Map final
    {
        unsigned width, height;
        unsigned tileW, tileH;
        unsigned version;
        SDL_Color bg;
        // TODO: add tiles
    };

    struct Layer final
    {
        std::vector<unsigned> data;
    };
} // namespace tnt::tmx

#endif //!TNT_DOO_TILED_MAPS_HPP