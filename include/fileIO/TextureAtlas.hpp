#ifndef TNT_ASSETS_TEXTURE_ATLAS_HPP
#define TNT_ASSETS_TEXTURE_ATLAS_HPP

#include "core/Config.hpp"
#include "math/Rectangle.hpp"
#include "core/Window.hpp"

namespace tnt
{
    class TNT_API TextureAtlas final
    {
    public:
        TextureAtlas(Window const &win, std::string_view file_, Rectangle const &area) noexcept;

        void Draw(Window const &win, Vector const &pos, Vector const &scale = {1.f, 1.f}, float angle = 0.f) noexcept;

    private:
        const char *filename;
        Rectangle clip;
    };
} // namespace tnt

#endif //!TNT_ASSETS_TEXTURE_ATLAS_HPP