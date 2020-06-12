#ifndef TNT_TILED_OBJECT_HPP
#define TNT_TILED_OBJECT_HPP

#include <span>
#include <vector>

#include "math/Vector.hpp"
#include "tiled/TiledProperty.hpp"

// TODO: different ways to draw text (italic, bold, underlined, etc)
// https://doc.mapeditor.org/en/stable/reference/json-map-format/#json-object-text

namespace tnt::tmx
{
    struct Object final
    {
        inline Object(std::span<tnt::Vector> polygon_) noexcept {}

        bool ellipse;
        bool point;
        bool visible;

        int gid;
        int id;

        double x;
        double y;
        double width;
        double height;
        double angle;

        std::string name;
        std::string templ;
        std::string text;
        std::string type;

        std::vector<tnt::Vector> polygon;
        std::vector<tnt::Vector> polyline;
        std::vector<tmx::Property> properties;
    };
} // namespace tnt::tmx

#endif //!TNT_TILED_OBJECT_HPP