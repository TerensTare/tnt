#ifndef TNT_TMX_MAP_LOADER_HPP
#define TNT_TMX_MAP_LOADER_HPP

#include "tiled/TileMap.hpp"

namespace tnt
{
namespace tmx
{
namespace detail
{

class MapLoadPolicy
{
public:
    virtual ~MapLoadPolicy() noexcept = default;

    virtual void *loadMap(std::string_view mapName) noexcept = 0;
    TileMap *getMap(std::string_view mapName) const noexcept;

protected:
    // friend class MapLoader<MapLoadPolicy>;
};

class XmlMapType final : public MapLoadPolicy
{
};

class JsonMapType final : public MapLoadPolicy
{
};

template <typename LoadPolicy>
class MapLoader
{
public:
    MapLoader();

private:
    LoadPolicy *policy;
};
} // namespace detail

using XmlMapLoader = tmx::detail::MapLoader<tmx::detail::XmlMapType>;
using JsonMapLoader = tmx::detail::MapLoader<tmx::detail::JsonMapType>;
} // namespace tmx
} // namespace tnt

#endif //!TNT_TMX_MAP_LOADER_HPP