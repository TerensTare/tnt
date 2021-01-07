#ifndef TNT_VIRTUAL_FILE_SYSTEM_HPP
#define TNT_VIRTUAL_FILE_SYSTEM_HPP

#include <map>
#include <mutex>
#include <string>

#include "core/Config.hpp"
#include "utils/Traits.hpp"

// TODO:
// Support for handling Zip archives, etc.
// thread-safe operations.

// TODO(maybe):
// extra argument to lock/unlock the mutex only once (for batch operations) ??
// separate alias directories from alias files ??
// constexpr ops ??

TNT_EXPORT template struct TNT_API std::less<void>;
TNT_EXPORT class TNT_API std::mutex;
TNT_EXPORT template class TNT_API std::map<char const *, char const *, std::less<>>;

namespace tnt
{
    class TNT_API vfs_handle final
    {
        /// @brief Make @a alias evaluate to @a path when used with @a tnt::vfs::absolute.
        /// @param path The path to be aliased.
        /// @param alias The alias of @a path.
        /// @note If @a alias has already been assigned to a different path, a debug log will be emmited and the old path will be replaced by the new one.
        /// @sa tnt::vfs::absolute.
        void mount(char const *path, char const *alias) noexcept;

        /// @brief Remove @a path from the defined aliases.
        /// @param path The alias path to be removed.
        /// @note If @a path was never assigned, a debug log will be emmited and nothing will happen.
        void unmount(char const *path) noexcept;

        /// @brief Get the absolute path from a(n) absolute/relative/aliased path.
        /// @param path The path to be turned to an absolute path.
        /// @return std::string
        std::string absolute(std::string_view path) const noexcept;

    private:
        mutable std::mutex mtx;
        std::map<const char *, char const *, std::less<>> entries;
    };
} // namespace tnt

namespace tnt::vfs
{
    /// @brief Return the path separator used by the OS. "\\" on Windows, "/" otherwise.
    /// @todo (maybe) Use consteval ??
    /// @return constexpr const char*
    constexpr const char *path_sep() noexcept
    {
        if constexpr (is_windows_v)
            return "\\";
        else
            return "/";
    }

    /// @brief Make @a alias evaluate to @a path when used with @a tnt::vfs::absolute.
    /// @param path The path to be aliased.
    /// @param alias The alias of @a path.
    /// @note If @a alias has already been assigned to a different path, a debug log will be emmited and the old path will be replaced by the new one.
    /// @sa tnt::vfs::absolute.
    TNT_API void mount(char const *path, char const *alias) noexcept;

    /// @brief Remove @a path from the defined aliases.
    /// @param path The alias path to be removed.
    /// @note If @a path was never assigned, a debug log will be emmited and nothing will happen.
    TNT_API void unmount(char const *path) noexcept;

    /// @brief Get the absolute path from a(n) absolute/relative/aliased path.
    /// @param path The path to be turned to an absolute path.
    /// @return std::string
    TNT_API std::string absolute(std::string_view path) noexcept;
} // namespace tnt::vfs

#endif //! TNT_VIRTUAL_FILE_SYSTEM_HPP