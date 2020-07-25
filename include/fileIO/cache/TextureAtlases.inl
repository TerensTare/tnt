#ifndef TNT_TEXTURE_ATLASES_ASSETS_CACHE_HPP
#define TNT_TEXTURE_ATLASES_ASSETS_CACHE_HPP

#include "fileIO/TextureAtlas.hpp"
#include "fileIO/cache/Base.hpp"

namespace tnt
{
    template <int I>
    class asset_cache<TextureAtlas, I>
    {
    public:
        [[nodiscard]] inline TextureAtlas *get(tnt::Window const &win, std::string_view path, Rectangle const &rect)
        {
            load(win, path, rect);
            return cache[path.data()];
        }

        inline void load(tnt::Window const &win, std::string_view path, Rectangle const &rect)
        {
            cache.try_emplace(win, path, rect);
        }

    private:
        std::byte memory[I * sizeof(TextureAtlas)];
        std::pmr::monotonic_buffer_resource res{memory, sizeof(memory)};
        std::pmr::map<std::string, TextureAtlas> cache{&res};
    };

    template <int I>
    using texture_atlas_cache = asset_cache<TextureAtlas, I>;

    using small_texture_atlas_cache = texture_atlas_cache<10>;
    using medium_texture_atlas_cache = texture_atlas_cache<50>;
    using large_texture_atlas_cache = texture_atlas_cache<100>;
} // namespace tnt

#endif //!TNT_TEXTURE_ATLASES_ASSETS_CACHE_HPP