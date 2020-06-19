#ifndef TNT_DOO_TILED_MAPS_HPP
#define TNT_DOO_TILED_MAPS_HPP

#include <map>
#include <string>
#include <vector>

#include "math/Vector.hpp"

namespace tnt::tmx
{
    namespace detail
    {
        enum tmx_bool
        {
            FALSE = 0,
            TRUE
        };
    }

    using tile_id_t = int;
    using property_t = std::map<std::string, std::string>;

    enum map_orientation
    {
        orthogonal = 0,
        isometric,
        staggered,
        hexagonal
    };

    enum render_order
    {
        right_down = 0,
        right_up,
        left_down,
        left_up
    };

    class TileMaps
    {
        std::vector<detail::tmx_bool> infinite;
        std::vector<std::string> backgroundcolors; // (optional)
        std::vector<tnt::Vector> sizes;            // width + height
        std::vector<tnt::Vector> tilesizes;        // tilew + tileh
        std::vector<property_t> properties;        // (maybe) the value should be std::string_view ??
    };

    class StaggeredMaps : public TileMaps
    {
        std::vector<detail::tmx_bool> staggered_axes;    // false if x, true if y
        std::vector<detail::tmx_bool> staggered_indices; // false if odd, true if even
    };

    class HexTileMaps : public TileMaps
    {
        std::vector<int> hexsidelengths;
        std::vector<detail::tmx_bool> staggered_axes;    // false if x, true if y
        std::vector<detail::tmx_bool> staggered_indices; // false if odd, true if even
    };
} // namespace tnt::tmx

#endif //!TNT_DOO_TILED_MAPS_HPP