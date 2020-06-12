#ifndef TNT_TILED_BASIC_TILE_HPP
#define TNT_TILED_BASIC_TILE_HPP

#include "tiled/TiledLayer.hpp"

namespace tnt::tmx
{
    struct Frame
    {
        inline Frame(int duration_, int id_) noexcept
            : duration{duration_}, id{id_} {}

        int duration;
        int id;
    };

    struct Tile final
    {
        inline Tile(int w, int h, std::span<tmx::Frame> anims,
                    std::span<tmx::Property> props, std::span<tmx::Layer> objects,
                    std::string_view image_ = "") noexcept
            : imagewidth{w}, imageheight{h}, image{image_},
              animation{anims.begin(), anims.end()},
              properties{props.begin(), props.end()},
              objectgroup{objects.begin(), objects.end()} {}

        int id;
        int imagewidth;
        int imageheight;

        std::string image; // optional

        std::vector<tmx::Frame> animation;
        std::vector<tmx::Property> properties;
        std::vector<tmx::Layer> objectgroup; // optional
    };
} // namespace tnt::tmx

#endif //!TNT_TILED_BASIC_TILE_HPP