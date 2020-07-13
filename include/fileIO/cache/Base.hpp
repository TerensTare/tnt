#ifndef TNT_FILE_IO_ASSETS_CACHE_BASE_HPP
#define TNT_FILE_IO_ASSETS_CACHE_BASE_HPP

#include <map>
#include <memory_resource>
#include <string_view>

namespace tnt
{
    template <typename T, int I>
    class asset_cache
    {
        inline constexpr asset_cache() noexcept = default;
    };

    namespace detail
    {
        auto to_abs = [](std::string_view path) noexcept {
            return std::string{SDL_GetBasePath()}.append(path);
        };
    }
} // namespace tnt

#endif //!TNT_FILE_IO_ASSETS_CACHE_BASE_HPP