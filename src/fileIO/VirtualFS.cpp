// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <filesystem>

#include "fileIO/VirtualFS.hpp"

tnt::VirtualFS::VirtualFS()
    : aliases { { ".", basePath },
        { "scripts", "." PATH_SEPARATOR "scripts" },
        { "data", "." PATH_SEPARATOR "data" },
        { "images", "." PATH_SEPARATOR "images" } }
{
}

tnt::VirtualFS::~VirtualFS() noexcept
{
    aliases.clear();
    std::map<std::string_view, std::string_view, std::less<>> {}.swap(aliases);
}

// tnt::VirtualFS &tnt::VirtualFS::This()
// {
//     static VirtualFS vfs;
//     return vfs;
// }

std::string_view tnt::VirtualFS::CurrentDirectory()
{
    return std::filesystem::current_path().string();
}

void tnt::VirtualFS::mount(std::string_view alias, std::string_view path)
{
    aliases.insert({ alias, path });
}

void tnt::VirtualFS::unmount(std::string_view alias)
{
    aliases.erase(alias);
}

std::string_view tnt::VirtualFS::get(std::string_view alias) const
{
    return aliases.at(alias);
}

std::string_view& tnt::VirtualFS::operator[](std::string_view const& alias)
{
    return aliases[alias];
}

void tnt::VirtualFS::clean()
{
    aliases.clear();
}

std::string_view tnt::VirtualFS::operator>>(std::string_view alias)
{
    return aliases[alias];
}

std::string_view tnt::VirtualFS::asDir(std::string_view path)
{
    std::string ret { path };
    ret.append(PATH_SEPARATOR);
    return ret.c_str();
}