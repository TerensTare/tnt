#ifndef TNT_ASSETS_TEXTURE_ATLAS_HPP
#define TNT_ASSETS_TEXTURE_ATLAS_HPP

#include "core/Config.hpp"
#include "core/Window.hpp"

#include "math/Rectangle.hpp"
#include "types/HashedString.hpp"

namespace tnt
{
    class TNT_API TextureAtlas final
    {
    public:
        TextureAtlas(Window const &win, std::string_view file_, Rectangle const &area) noexcept;

        void Draw(Window const &win, Vector const &pos, Vector const &scale = {1.f, 1.f}, float angle = 0.f) noexcept;

    private:
        tnt::hashed_string filename;
        Rectangle clip;
    };
} // namespace tnt

#endif //!TNT_ASSETS_TEXTURE_ATLAS_HPP