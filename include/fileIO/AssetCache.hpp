#ifndef TNT_ASSETS_CACHE_HPP
#define TNT_ASSETS_CACHE_HPP

#include "fileIO/cache/Textures.inl"
#include "fileIO/cache/TextureAtlases.inl"
#include "fileIO/cache/Fonts.inl"
#include "fileIO/cache/Music.inl"

// TODO:
// make the default asset_cache-s constinit.

// TODO(maybe):
// add set_default_*asset*_cache ??

namespace tnt::detail
{
    inline small_font_cache def_font_cache{};
    inline small_music_cache def_music_cache{};
    inline small_sfx_cache def_sfx_cache{};
    inline medium_texture_cache def_texture_cache{};
    inline small_texture_atlas_cache def_texture_atlas_cache{};
} // namespace tnt::detail

namespace tnt
{
    inline small_font_cache *default_font_cache() noexcept { return &detail::def_font_cache; }
    inline small_music_cache *default_music_cache() noexcept { return &detail::def_music_cache; }
    inline small_sfx_cache *default_sfx_cache() noexcept { return &detail::def_sfx_cache; }
    inline medium_texture_cache *default_texture_cache() noexcept { return &detail::def_texture_cache; }
    inline small_texture_atlas_cache *default_texture_atlas_cache() noexcept { return &detail::def_texture_atlas_cache; }
} // namespace tnt

#endif //!TNT_ASSETS_CACHE_HPP