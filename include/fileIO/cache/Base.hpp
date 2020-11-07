#ifndef TNT_FILE_IO_ASSETS_CACHE_BASE_HPP
#define TNT_FILE_IO_ASSETS_CACHE_BASE_HPP

namespace tnt
{
    template <typename T, unsigned I>
    struct asset_cache
    {
        constexpr asset_cache() noexcept = default;
    };
} // namespace tnt

#endif //!TNT_FILE_IO_ASSETS_CACHE_BASE_HPP