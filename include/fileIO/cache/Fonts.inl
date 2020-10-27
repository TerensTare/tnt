#ifndef TNT_FONTS_ASSETS_CACHE_INL
#define TNT_FONTS_ASSETS_CACHE_INL

#include <map>
#include <memory_resource>
#include <string_view>

#include <SDL2/SDL_ttf.h>

#include "core/Config.hpp"
#include "fileIO/cache/Base.hpp"
#include "fileIO/VirtualFS.hpp"

namespace tnt
{
    template <unsigned I>
    class TNT_API asset_cache<TTF_Font, I>
    {
    public:
        inline ~asset_cache() noexcept
        {
            for (auto const &it : cache)
                TTF_CloseFont(it.second);
        }

        [[nodiscard]] inline TTF_Font *get(std::string_view path, int size)
        {
            load(path, size);
            return cache[vfs::absolute(path)];
        }

        inline void load(std::string_view path, int size)
        {
            cache.try_emplace(vfs::absolute(path), TTF_OpenFont(vfs::absolute(path).c_str(), size));
        }

    private:
        std::byte memory[I * sizeof(TTF_Font *)];
        std::pmr::monotonic_buffer_resource res{memory, sizeof(memory)};
        std::pmr::unordered_map<std::string, TTF_Font *> cache{&res};
    };

    template <unsigned I>
    using font_cache = asset_cache<TTF_Font, I>;

    using small_font_cache = font_cache<10>;
    using medium_font_cache = font_cache<50>;
    using large_font_cache = font_cache<100>;
} // namespace tnt

#endif //!TNT_FONTS_ASSETS_CACHE_INL