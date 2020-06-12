#ifndef TNT_TILED_GRID_HPP
#define TNT_TILED_GRID_HPP

#include <string>

namespace tnt::tmx
{
    struct Grid final
    {
        inline Grid(int width_, int height_,
                    std::string_view orientation_ = "orthogonal") noexcept
            : width{width_}, height{height_}, orientation{orientation_} {}

        int width;
        int height;

        std::string orientation;
    };
} // namespace tnt::tmx

#endif //!TNT_TILED_GRID_HPP