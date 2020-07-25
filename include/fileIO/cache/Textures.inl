#ifndef TNT_TEXTURES_ASSETS_CACHE_INL
#define TNT_TEXTURES_ASSETS_CACHE_INL

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "fileIO/cache/Base.hpp"

namespace tnt
{
    template <int I>
    class asset_cache<SDL_Texture, I>
    {
    public:
        inline ~asset_cache() noexcept
        {
            for (auto const &it : cache)
                SDL_DestroyTexture(it.second);
        }

        [[nodiscard]] inline SDL_Texture *get(SDL_Renderer *ren,
                                              std::string_view path)
        {
            load(ren, path);
            return cache[path.data()];
        }

        inline void load(SDL_Renderer *ren, std::string_view path)
        {
            cache.try_emplace(path.data(), IMG_LoadTexture(ren, detail::to_abs(path.data()).c_str()));
        }

    private:
        std::byte memory[I * sizeof(SDL_Texture *)];
        std::pmr::monotonic_buffer_resource res{memory, sizeof(memory)};
        std::pmr::map<std::string, SDL_Texture *> cache{&res};
    };

    template <int I>
    using texture_cache = asset_cache<SDL_Texture, I>;

    using small_texture_cache = texture_cache<10>;
    using medium_texture_cache = texture_cache<50>;
    using large_texture_cache = texture_cache<100>;
} // namespace tnt

#endif //!TNT_TEXTURES_ASSETS_CACHE_INL