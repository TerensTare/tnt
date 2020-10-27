#ifndef TNT_TEXTURE_ATLASES_ASSETS_CACHE_HPP
#define TNT_TEXTURE_ATLASES_ASSETS_CACHE_HPP

#include <unordered_map>
#include <memory_resource>

#include "core/Config.hpp"
#include "fileIO/TextureAtlas.hpp"
#include "fileIO/cache/Base.hpp"

namespace tnt
{
    template <unsigned I>
    class TNT_API asset_cache<TextureAtlas, I>
    {
    public:
        [[nodiscard]] inline TextureAtlas *get(tnt::Window const &win, std::string_view path, Rectangle const &rect)
        {
            load(win, path, rect);
            return cache[vfs::absolute(path)];
        }

        inline void load(tnt::Window const &win, std::string_view path, Rectangle const &rect)
        {
            cache.try_emplace(vfs::absolute(path), win, path, rect);
        }

    private:
        std::byte memory[I * sizeof(TextureAtlas)];
        std::pmr::monotonic_buffer_resource res{memory, sizeof(memory)};
        std::pmr::unordered_map<std::string, TextureAtlas> cache{&res};
    };

    template <unsigned I>
    using texture_atlas_cache = asset_cache<TextureAtlas, I>;

    using small_texture_atlas_cache = texture_atlas_cache<10>;
    using medium_texture_atlas_cache = texture_atlas_cache<50>;
    using large_texture_atlas_cache = texture_atlas_cache<100>;
} // namespace tnt

#endif //!TNT_TEXTURE_ATLASES_ASSETS_CACHE_HPP