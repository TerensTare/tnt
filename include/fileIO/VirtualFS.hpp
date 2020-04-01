#ifndef TNT_VIRTUAL_FILE_SYSTEM_HPP
#define TNT_VIRTUAL_FILE_SYSTEM_HPP

#include <map>
#include <string_view>

#include "core/Config.hpp"

// TODO: check if alias exists in get() function and return "\"\"" if it doesn't.
// TODO: Consider making the map with value=std::filesystem::path

// TODO(maybe):
// use an unordered_map ??

namespace tnt
{
class VirtualFS
{
public:
    VirtualFS();
    ~VirtualFS() noexcept;

    // VirtualFS(VirtualFS const &) = delete;
    // VirtualFS &operator=(VirtualFS const &) = delete;

    // static VirtualFS &This();

    void mount(std::string_view alias, std::string_view path);
    void unmount(std::string_view alias);

    std::string_view CurrentDirectory();

    std::string_view get(std::string_view alias) const;

    std::string_view &operator[](std::string_view const &alias);

    std::string_view path(std::string_view alias, std::string_view p);

    void clean();

    std::string_view operator>>(std::string_view alias);

private:
    std::string_view asDir(std::string_view path);

    std::string_view basePath{"." PATH_SEPARATOR}; // for now
    std::map<std::string_view, std::string_view, std::less<>> aliases;
};
} // namespace tnt

#endif //!TNT_VIRTUAL_FILE_SYSTEM_HPP