#ifndef TNT_FILE_IO_ASSETS_CACHE_BASE_HPP
#define TNT_FILE_IO_ASSETS_CACHE_BASE_HPP

#include "core/Config.hpp"

namespace tnt
{
    template <typename T, unsigned I>
    struct TNT_API asset_cache
    {
        constexpr asset_cache() noexcept = default;
    };
} // namespace tnt

#endif //!TNT_FILE_IO_ASSETS_CACHE_BASE_HPP