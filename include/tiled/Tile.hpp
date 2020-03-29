#ifndef TNT_TMX_TILE_HPP
#define TNT_TMX_TILE_HPP

#include <string>
#include <unordered_map>

namespace tnt::tmx
{
class Tile final
{
public:
    Tile(unsigned id_, std::unordered_map<std::string, std::string> const &props);
    ~Tile() noexcept;

    unsigned getID() const noexcept;
    std::string getProperty(std::string const &name) noexcept;

private:
    unsigned id;
    std::unordered_map<std::string, std::string> properties;
};
} // namespace tnt::tmx

#endif //!TNT_TMX_TILE_HPP