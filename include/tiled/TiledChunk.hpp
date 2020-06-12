#ifndef TNT_TILED_CHUNK_HPP
#define TNT_TILED_CHUNK_HPP

#include <span>
#include <vector>

// TODO(maybe):
// move this to TiledLayer.hpp ??

namespace tnt::tmx
{
    struct Chunk final
    {
        inline Chunk(int x_, int y_, int w_, int h_,
                     std::span<unsigned> data_) noexcept
            : x{x_}, y{y_}, width{w_}, height{h_},
              data{data_.cbegin(), data_.cend()} {}

        int x;
        int y;
        int width;
        int height;

        std::vector<unsigned> data;
    };
} // namespace tnt::tmx

#endif //!TNT_TILED_CHUNK_HPP