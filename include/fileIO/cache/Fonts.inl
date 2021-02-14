#ifndef TNT_FONTS_ASSETS_CACHE_INL
#define TNT_FONTS_ASSETS_CACHE_INL

#include <memory_resource>
#include <unordered_map>

#include <SDL2/SDL_ttf.h>

#include "fileIO/cache/Base.hpp"
#include "fileIO/VirtualFS.hpp"

#include "types/HashedString.hpp"

namespace tnt
{
    template <unsigned I>
    class asset_cache<TTF_Font, I>
    {
    public:
        inline ~asset_cache() noexcept
        {
            for (auto const &it : cache)
                if (it.second != nullptr)
                    TTF_CloseFont(it.second);
        }

        [[nodiscard]] inline TTF_Font *get(std::string_view path, int size)
        {
            load(path, size);
            return cache[fnv1a<char>(vfs::absolute(path))];
        }

        inline void load(std::string_view path, int size)
        {
            tnt::hashed_string p{vfs::absolute(path).c_str()};
            cache.try_emplace(p, TTF_OpenFont(p.data(), size));
        }

        inline void remove(std::string_view path)
        {
            if (tnt::hashed_string p{vfs::absolute(path).c_str()};
                cache.contains(p))
            {
                TTF_CloseFont(cache[p]);
                cache.erase(p);
                cache[p] = nullptr;
            }
        }

    private:
        std::byte memory[I * sizeof(TTF_Font *)];
        std::pmr::monotonic_buffer_resource res{memory, sizeof(memory)};
        std::pmr::unordered_map<
            typename tnt::hashed_string::hash_type, TTF_Font *>
            cache{&res};
    };

    template <unsigned I>
    using font_cache = asset_cache<TTF_Font, I>;

    using small_font_cache = font_cache<10>;
    using medium_font_cache = font_cache<50>;
    using large_font_cache = font_cache<100>;
} // namespace tnt

#endif //!TNT_FONTS_ASSETS_CACHE_INL