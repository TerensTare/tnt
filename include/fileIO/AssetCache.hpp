#ifndef TNT_ASSETS_CACHE_HPP
#define TNT_ASSETS_CACHE_HPP

#include "fileIO/cache/Textures.inl"
#include "fileIO/cache/Fonts.inl"
#include "fileIO/cache/Music.inl"

// TODO:
// make the default asset_cache-s constinit.

// TODO(maybe):
// add set_default_*asset*_cache ??

namespace tnt::detail
{
    inline medium_font_cache def_font_cache{};
    inline medium_music_cache def_music_cache{};
    inline medium_sfx_cache def_sfx_cache{};
    inline medium_texture_cache def_texture_cache{};
} // namespace tnt::detail

namespace tnt
{
    inline medium_font_cache *default_font_cache() noexcept { return &detail::def_font_cache; }
    inline medium_music_cache *default_music_cache() noexcept { return &detail::def_music_cache; }
    inline medium_sfx_cache *default_sfx_cache() noexcept { return &detail::def_sfx_cache; }
    inline medium_texture_cache *default_texture_cache() noexcept { return &detail::def_texture_cache; }
} // namespace tnt

#endif //!TNT_ASSETS_CACHE_HPP