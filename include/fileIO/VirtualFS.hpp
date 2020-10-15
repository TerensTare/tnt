#ifndef TNT_VIRTUAL_FILE_SYSTEM_HPP
#define TNT_VIRTUAL_FILE_SYSTEM_HPP

#include <string>

// TODO:
// Support for handling Zip archives, etc.
// thread-safe operations.

// TODO(maybe):
// separate alias directories from alias files ??
// suffix directory aliases with :// (ex. res://) ??
// constexpr ops ??

namespace tnt::vfs
{
    /// @brief Return the path separator used by the OS. "\\" on Windows, "/" otherwise.
    /// @todo (maybe) Use consteval ??
    /// @return constexpr const char const*
    constexpr const char *path_sep() noexcept;

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
    std::string absolute(std::string_view path) noexcept;
} // namespace tnt::vfs

#endif //! TNT_VIRTUAL_FILE_SYSTEM_HPP