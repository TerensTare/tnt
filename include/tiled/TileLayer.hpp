#ifndef TNT_TMX_TILE_LAYER_HPP
#define TNT_TMX_TILE_LAYER_HPP

#include <vector>
#include <unordered_map>

namespace tnt::tmx
{
class TileLayer final
{
public:
    TileLayer(std::string const &layerName, unsigned width, unsigned height,
              std::unordered_map<std::string, std::string> layerProps,
              std::vector<std::vector<unsigned>> tilesVector);
    ~TileLayer() noexcept;

    unsigned getWidth() const noexcept;
    unsigned getHeight() const noexcept;
    std::string getName() const noexcept;
    std::string getProperty(std::string const &prop) noexcept;
    std::vector<std::vector<unsigned>> getTiles() const noexcept;

private:
    unsigned w, h;
    std::string name;
    std::vector<std::vector<unsigned>> tiles;
    std::unordered_map<std::string, std::string> properties;
};
} // namespace tnt::tmx

#endif //!TNT_TMX_TILE_LAYER_HPP