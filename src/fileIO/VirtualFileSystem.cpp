// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <filesystem>

#include "VirtualFileSystem.hpp"

tnt::VirtualFileSystem::VirtualFileSystem()
    : aliases{{".", basePath},
              {"scripts", ".\\scripts"},
              {"data", ".\\data"},
              {"images", ".\\images"}}
{
}

tnt::VirtualFileSystem::~VirtualFileSystem() noexcept
{
    aliases.clear();
    std::map<std::string_view, std::string_view, std::less<>>{}.swap(aliases);
}

tnt::VirtualFileSystem &tnt::VirtualFileSystem::This()
{
    static VirtualFileSystem vfs;
    return vfs;
}

std::string_view tnt::VirtualFileSystem::CurrentDirectory()
{
    return std::filesystem::current_path().string();
}

void tnt::VirtualFileSystem::mount(std::string_view alias, std::string_view path)
{
    aliases.insert({alias, path});
}

void tnt::VirtualFileSystem::unmount(std::string_view alias)
{
    aliases.erase(alias);
}

std::string_view tnt::VirtualFileSystem::get(std::string_view alias) const
{
    return aliases.at(alias);
}

std::string_view &tnt::VirtualFileSystem::operator[](std::string_view const &alias)
{
    return aliases[alias];
}

void tnt::VirtualFileSystem::clean()
{
    aliases.clear();
}

std::string_view tnt::VirtualFileSystem::operator>>(std::string_view alias)
{
    return aliases[alias];
}