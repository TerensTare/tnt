#ifndef TNT_CONTEXT_HPP
#define TNT_CONTEXT_HPP

#include <variant>
#include <sol/sol.hpp>
#include "tiled/MapLoader.hpp"

namespace tnt
{
    class Window;
    class VirtualFS;
    class AssetManager;

    // TODO: add threadsafe logger.

    // TODO(maybe):
    // add Timer* timer ??
    // remove dependecy of "tiled/MapLoader.hpp" ??

    struct Context
    {
        ~Context() noexcept;
        std::weak_ptr<Window> window;
        std::variant<tmx::JsonMapLoader *, tmx::XmlMapLoader *> mapLoader;
        sol::state lua;
        AssetManager *assets;
    };

    // Context &defaultCtx;
} // namespace tnt

#endif //!TNT_CONTEXT_HPP