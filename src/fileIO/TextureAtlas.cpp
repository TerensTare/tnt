// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "fileIO/TextureAtlas.hpp"
#include "fileIO/cache/Textures.inl"

inline static tnt::texture_cache<3> *cache;

tnt::TextureAtlas::TextureAtlas(tnt::Window const &win, std::string_view file, tnt::Rectangle const &area) noexcept
    : filename{file}, clip{area}
{
    cache->load(win.getRenderer(), file);
}

void tnt::TextureAtlas::Draw(tnt::Window const &win, tnt::Vector const &pos, tnt::Vector const &scale, float angle) noexcept
{
    SDL_FRect const &dst{pos.x, pos.y, clip.w * scale.x, clip.h * scale.y};
    SDL_Texture *tex{cache->get(win.getRenderer(), filename)};
    SDL_Rect const &from{(SDL_Rect)clip};
    SDL_RenderCopyExF(win.getRenderer(), tex, &from, &dst, angle, nullptr, SDL_FLIP_NONE);
}