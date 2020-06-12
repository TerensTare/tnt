#ifndef TNT_TILED_LAYERS_HPP
#define TNT_TILED_LAYERS_HPP

#include "tiled/TiledChunk.hpp"
#include "tiled/TiledProperty.hpp"

// TODO: remove std::optional dependency.
// TODO: replace std::string with sth lighter (ex. std::string_view).
// TODO: Make tmx::Object "fully functional".

// TODO(maybe):
// Don't expose the id of the tmx:: types, or even remove it ??
// Group all "small" structs like Chunk, Frame on a single file ??
// Remove all those "name" members ??
// Treat tmx::Layer as an union of different layer types ??
// Do the same thing with other tmx:: types that have optional data.
// Use enums to represent the "type" of a tmx:: member ??

namespace tnt::tmx
{
    struct Layer final
    {
        inline Layer(
            bool visible_, int width_, int height_, int id_,
            int startx_, int starty_, std::string_view draworder_,
            std::string_view image_, std::string_view name_,
            std::string_view type_, unsigned *transparentcolor_,
            std::span<unsigned> data_, std::span<tmx::Property> props_,
            std::span<tmx::Chunk> chunks_) noexcept
            : visible{visible_}, width{width_}, height{height_}, id{id_},
              startx{startx_}, starty{starty_}, draworder{draworder_},
              image{image_}, name{name_}, type{type_},
              transparentcolor{transparentcolor_}, data{data_.begin(), data_.end()},
              properties{props_.begin(), props_.end()},
              chunks{chunks_.begin(), chunks_.end()} {}

        bool visible;

        int width;
        int height;
        int id;
        int startx;
        int starty;
        const int x{0};
        const int y{0};

        double offsetx{0};
        double offsety{0};
        double opacity;

        std::string compression{''};
        std::string draworder{"topdown"};
        std::string encoding{"csv"};
        std::string image;
        std::string name;
        std::string type;

        unsigned transparentcolor[4];
        std::vector<unsigned> data;
        std::vector<tmx::Property> properties;
        std::vector<tmx::Chunk> chunks;
        std::vector<tmx::Layer *> layers;
    };
} // namespace tnt::tmx

#endif //!TNT_TILED_LAYERS_HPP