#ifndef TNT_TEXTURES_ASSETS_CACHE_INL
#define TNT_TEXTURES_ASSETS_CACHE_INL

#include <unordered_map>
#include <memory_resource>

#include <SDL2/SDL_image.h>

#include "fileIO/cache/Base.hpp"
#include "fileIO/VirtualFS.hpp"

#include "types/HashedString.hpp"

namespace tnt
{
    template <unsigned I>
    class asset_cache<SDL_Texture, I>
    {
    public:
        inline ~asset_cache() noexcept
        {
            for (auto const &it : cache)
                if (it.second != nullptr)
                    SDL_DestroyTexture(it.second);
        }

        [[nodiscard]] inline SDL_Texture *get(SDL_Renderer *ren,
                                              std::string_view path)
        {
            load(ren, path);
            return cache[fnv1a<char>(vfs::absolute(path))];
        }

        inline void load(SDL_Renderer *ren, std::string_view path)
        {
            hashed_string str{vfs::absolute(path)};
            cache.try_emplace(str.hash(), IMG_LoadTexture(ren, str.data()));
        }

        inline void remove(std::string_view path)
        {
            if (hashed_string str{vfs::absolute(path)};
                cache.contains(str))
            {
                SDL_DestroyTexture(cache[str]);
                cache.erase(str);
                cache[str] = nullptr;
            }
        }

    private:
        std::byte memory[I * sizeof(SDL_Texture *)];
        std::pmr::monotonic_buffer_resource res{memory, sizeof(memory)};
        std::pmr::unordered_map<
            typename tnt::hashed_string::hash_type, SDL_Texture *>
            cache{&res};
    };

    template <unsigned I>
    using texture_cache = asset_cache<SDL_Texture, I>;

    using small_texture_cache = texture_cache<10>;
    using medium_texture_cache = texture_cache<50>;
    using large_texture_cache = texture_cache<100>;
} // namespace tnt

#endif //!TNT_TEXTURES_ASSETS_CACHE_INL