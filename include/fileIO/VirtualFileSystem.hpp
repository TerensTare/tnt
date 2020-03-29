#ifndef VIRTUAL_FILE_SYSTEM_HPP
#define VIRTUAL_FILE_SYSTEM_HPP

#include <map>
#include <string_view>

#include "core/Config.hpp"

// TODO:
// check if alias exists in get() function and return "\"\"" if it doesn't.
// Consider making the map with value=std::filesystem::path

namespace tnt
{
class VirtualFileSystem
{
public:
    VirtualFileSystem(VirtualFileSystem const &) = delete;
    void operator=(VirtualFileSystem const &) = delete;

    static VirtualFileSystem &This();

    void mount(std::string_view alias, std::string_view path);
    void unmount(std::string_view alias);

    std::string_view CurrentDirectory();

    std::string_view get(std::string_view alias) const;

    std::string_view &operator[](std::string_view const &alias);

    std::string_view path(std::string_view alias, std::string_view p);

    void clean();

    std::string_view operator>>(std::string_view alias);

private:
    VirtualFileSystem();
    ~VirtualFileSystem() noexcept;

    std::string_view asDir(std::string_view path)
    {
        std::string ret{path};
        ret.append(PATH_SEPARATOR);
        return ret.operator std::basic_string_view<char, std::char_traits<char>>();
    }

    std::string_view basePath{"."}; // for now
    std::map<std::string_view, std::string_view, std::less<>> aliases;
};
} // namespace tnt

#endif //!VIRTUAL_FILE_SYSTEM_HPP