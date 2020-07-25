#ifndef TNT_FONTS_ASSETS_CACHE_INL
#define TNT_FONTS_ASSETS_CACHE_INL

#include <SDL2/SDL_ttf.h>

#include "fileIO/cache/Base.hpp"

namespace tnt
{
    template <int I>
    class asset_cache<TTF_Font, I>
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
            return cache[path.data()];
        }

        inline void load(std::string_view path, int size)
        {
            cache.try_emplace(path.data(), TTF_OpenFont(detail::to_abs(path.data()).c_str(), size));
        }

    private:
        std::byte memory[I * sizeof(TTF_Font *)];
        std::pmr::monotonic_buffer_resource res{memory, sizeof(memory)};
        std::pmr::map<std::string, TTF_Font *> cache{&res};
    };

    template <int I>
    using font_cache = asset_cache<TTF_Font, I>;

    using small_font_cache = font_cache<10>;
    using medium_font_cache = font_cache<50>;
    using large_font_cache = font_cache<100>;
} // namespace tnt

#endif //!TNT_FONTS_ASSETS_CACHE_INL