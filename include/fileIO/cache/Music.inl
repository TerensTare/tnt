#ifndef TNT_MUSIC_SFX_ASSET_CACHE_INL
#define TNT_MUSIC_SFX_ASSET_CACHE_INL

#include <SDL2/SDL_mixer.h>

#include "fileIO/cache/Base.hpp"

namespace tnt
{
    template <unsigned I>
    class asset_cache<Mix_Music, I>
    {
    public:
        inline ~asset_cache() noexcept
        {
            for (auto const &it : cache)
                if (it.second != nullptr)
                    Mix_FreeMusic(it.second);
        }

        inline Mix_Music *get(std::string_view path)
        {
            load(path);
            return cache[vfs::absolute(path)];
        }

        inline void load(std::string_view path)
        {
            const char *p{vfs::absolute(path).c_str()};
            cache.try_emplace(p, Mix_LoadMUS(p));
        }

        inline void remove(std::string_view path)
        {
            if (const char *p{vfs::absolute(path).c_str()};
                cache.contains(p))
            {
                Mix_FreeMusic(cache[p]);
                cache.erase(p);
                cache[p] = nullptr;
            }
        }

    private:
        std::byte memory[I * sizeof(Mix_Music *)];
        std::pmr::monotonic_buffer_resource res{memory, sizeof(memory)};
        std::pmr::unordered_map<std::string, Mix_Music *> cache{&res};
    };

    template <unsigned I>
    using music_cache = asset_cache<Mix_Music, I>;

    using small_music_cache = music_cache<10>;
    using medium_music_cache = music_cache<50>;
    using large_music_cache = music_cache<100>;

    template <unsigned I>
    class asset_cache<Mix_Chunk, I>
    {
    public:
        inline ~asset_cache() noexcept
        {
            for (auto const &it : cache)
                if (it.second != nullptr)
                    Mix_FreeChunk(it.second);
        }

        [[nodiscard]] inline Mix_Chunk *get(std::string_view path)
        {
            load(path);
            return cache[vfs::absolute(path)];
        }

        inline void load(std::string_view path)
        {
            const char *p{vfs::absolute(path).c_str()};
            cache.try_emplace(p, Mix_LoadWAV(p));
        }

        inline void remove(std::string_view path)
        {
            if (const char *p{vfs::absolute(path).c_str()};
                cache.contains(p))
            {
                Mix_FreeChunk(cache[p]);
                cache.erase(p);
                cache[p] = nullptr;
            }
        }

    private:
        std::byte memory[I * sizeof(Mix_Chunk *)];
        std::pmr::monotonic_buffer_resource res{memory, sizeof(memory)};
        std::pmr::unordered_map<std::string, Mix_Chunk *> cache{&res};
    };

    template <int I>
    using sfx_cache = asset_cache<Mix_Chunk, I>;

    using small_sfx_cache = sfx_cache<10>;
    using medium_sfx_cache = sfx_cache<50>;
    using large_sfx_cache = sfx_cache<100>;
} // namespace tnt

#endif //!TNT_MUSIC_SFX_ASSET_CACHE_INL