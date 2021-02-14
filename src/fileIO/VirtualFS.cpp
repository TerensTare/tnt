// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <filesystem>

#include "fileIO/VirtualFS.hpp"
#include "utils/Logger.hpp"

namespace tnt
{
    void vfs_handle::mount(char const *path, char const *alias) noexcept
    {
        mtx.lock();

        if (tnt::hashed_string str{alias}; !entries.contains(str))
            entries[str] = path;
        else
            logger::debug("vfs: Replacing alias path {} with a new path.", alias);

        mtx.unlock();
    }

    void vfs_handle::unmount(char const *path) noexcept
    {
        mtx.lock();

        if (tnt::hashed_string str{path}; entries.contains(str))
            entries.erase(str);
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
        if (tnt::hashed_string str{path.data()}; entries.count(str) > 0)
            return entries.at(str);

        // if it is an alias + some other path, check it's validity.
        if (std::string_view::size_type const &it{path.find("://")};
            it != path.npos)
        {
            std::string const &alias{path.data(), it};
            if (auto const &h{fnv1a<char>(alias)};
                entries.count(h) > 0)
                return base + entries.find(h)->second +
                       vfs::path_sep() + (path.data() + it + 3);
            logger::error("vfs_handle: Using non-existent alias \"{}\" "
                          "with absolute()!!",
                          alias);
        }

        return base + path.data();
    }
} // namespace tnt

namespace tnt::vfs
{
    inline static std::unordered_map<typename hashed_string::hash_type, char const *> entries{};

    void mount(char const *path, char const *alias) noexcept
    {
        if (tnt::hashed_string str{alias}; !entries.contains(str))
            entries[str] = path;
        else
            logger::debug("vfs: Replacing alias path {} with a new path.", alias);
    }

    void unmount(char const *path) noexcept
    {
        if (tnt::hashed_string str{path}; entries.contains(str))
            entries.erase(str);
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
        if (tnt::hashed_string str{path.data()}; entries.count(str) > 0)
            return entries[str];

        // if it is an alias + some other path, check it's validity.
        if (std::string_view::size_type const &it{path.find("://")};
            it != path.npos)
        {
            std::string const &alias{path.data(), it};
            if (auto const &h{fnv1a<char>(alias)};
                entries.count(h) > 0)
                return base + entries.find(h)->second +
                       vfs::path_sep() + (path.data() + it + 3);
            logger::error("vfs: Using non-existent alias \"{}\" "
                          "with absolute()!!",
                          alias);
        }

        return base + path.data();
    }
} // namespace tnt::vfs