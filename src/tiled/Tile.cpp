// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "tiled/Tile.hpp"
#include "utils/Logger.hpp"

tnt::tmx::Tile::Tile(unsigned id_, std::unordered_map<std::string, std::string> const &props)
    : id{id_}, properties{props} {}

tnt::tmx::Tile::~Tile() noexcept
{
    properties.clear();
    std::unordered_map<std::string, std::string>{}.swap(properties);
}

unsigned tnt::tmx::Tile::getID() const noexcept { return id; }

std::string tnt::tmx::Tile::getProperty(std::string const &name) noexcept
{
    if (auto it{properties.find(name)}; it != properties.end())
        return it->second;
    tnt::logger::debug("Trying to access property {}, which doesn't exist on the Tile", name);
    return nullptr;
}