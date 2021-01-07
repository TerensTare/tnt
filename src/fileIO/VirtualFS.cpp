// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <map>

#include <filesystem>

#include "fileIO/VirtualFS.hpp"

#include "utils/Logger.hpp"
#include "utils/Traits.hpp"

namespace tnt
{
    void vfs_handle::mount(char const *path, char const *alias) noexcept
    {
        mtx.lock();

        if (!entries.contains(alias))
            entries[alias] = path;
        else
            logger::debug("vfs: Replacing alias path {} with a new path.", alias);

        mtx.unlock();
    }

    void vfs_handle::unmount(char const *path) noexcept
    {
        mtx.lock();

        if (entries.contains(path))
            entries.erase(path);
        else
            logger::debug("vfs: Trying to unmount() a non-existent alias path. Nothing will happen.");

        mtx.unlock();
    }

    std::string vfs_handle::absolute(std::string_view path) const noexcept
    {
        // TODO: check invalid paths.
        // TODO: handle cases when the path is absolute by default.

        // If the first character is '.', the path is NOT an alias
        std::string const &base{std::filesystem::current_path().string() + vfs::path_sep()};

        if (path.starts_with('.'))
            return base + (path.data() + 2);

        std::lock_guard _{mtx};

        // Otherwise, the path must be an alias. Check if it is just an alias or it is an alias + some other path.
        if (entries.count(path) > 0)
            return entries.at(path.data());

        // if it is an alias + some other path, check it's validity.
        if (std::string_view::size_type const &it{path.find("://")};
            it != path.npos)
        {
            if (std::string const &alias{path.data(), it};
                entries.count(alias) > 0)
                return base + entries.find(alias)->second +
                       vfs::path_sep() + (path.data() + it + 3);
            else
                logger::error("vfs: Using non-existent alias \"{}\" "
                              "with absolute()!!",
                              alias);
        }

        return base + path.data();
    }
} // namespace tnt

namespace tnt::vfs
{
    inline static std::map<char const *, char const *, std::less<>> entries{};

    void mount(char const *path, char const *alias) noexcept
    {
        if (!entries.contains(alias))
            entries[alias] = path;
        else
            logger::debug("vfs: Replacing alias path {} with a new path.", alias);
    }

    void unmount(char const *path) noexcept
    {
        if (entries.contains(path))
            entries.erase(path);
        else
            logger::debug("vfs: Trying to unmount() a non-existent alias path. Nothing will happen.");
    }

    std::string absolute(std::string_view path) noexcept
    {
        // TODO: check invalid paths.
        // TODO: handle cases when the path is absolute by default.

        // If the first character is '.', the path is NOT an alias
        std::string const &base{std::filesystem::current_path().string() + path_sep()};

        if (path.starts_with('.'))
            return base + (path.data() + 2);

        // Otherwise, the path must be an alias. Check if it is just an alias or it is an alias + some other path.
        if (entries.count(path) > 0)
            return entries.at(path.data());

        // if it is an alias + some other path, check it's validity.
        if (std::string_view::size_type const &it{path.find("://")};
            it != path.npos)
        {
            if (std::string const &alias{path.data(), it};
                entries.count(alias) > 0)
                return base + entries.find(alias)->second +
                       path_sep() + (path.data() + it + 3);
            else
                logger::error("vfs: Using non-existent alias \"{}\" "
                              "with absolute()!!",
                              alias);
        }

        return base + path.data();
    }
} // namespace tnt::vfs