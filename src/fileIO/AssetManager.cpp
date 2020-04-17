// This is an independent project of an individual developer. Dear PVS-Studio,
// please check it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "fileIO/AssetManager.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
// #include "tiled/TileMap.hpp"
#include "core/Window.hpp"

// TODO:
// use tnt::logger instead here.

tnt::AssetManager::AssetManager() {}

tnt::AssetManager::~AssetManager()
{
    for (auto const& it : images)
        if (it.second != nullptr)
            SDL_DestroyTexture(it.second);
    images.clear();
    std::map<std::string, SDL_Texture*>{}.swap(images);

    for (auto const& it : text)
        if (it.second != nullptr)
            SDL_DestroyTexture(it.second);
    text.clear();
    std::map<std::string, SDL_Texture*>{}.swap(text);

    for (auto const& it : fonts)
        if (it.second != nullptr)
            TTF_CloseFont(it.second);
    fonts.clear();
    std::map<std::string, TTF_Font*>{}.swap(fonts);

    for (auto const& it : music)
        if (it.second != nullptr)
            Mix_FreeMusic(it.second);
    music.clear();
    std::map<std::string, Mix_Music*>{}.swap(music);

    for (auto const& it : sfx)
        if (it.second != nullptr)
            Mix_FreeChunk(it.second);
    sfx.clear();
    std::map<std::string, Mix_Chunk*>{}.swap(sfx);

    // for (auto const&it : maps)
    //     if (it.second != nullptr)
    //         maps.erase(it);
    // maps.clear();
    // std::map<std::string, tmx::TileMap *>{}.swap(maps);
}

tnt::AssetManager& tnt::AssetManager::This()
{
    static AssetManager inst;
    return inst;
}

// SDL_Texture *tnt::AssetManager::LoadText(tnt::Window const *win, TTF_Font
// *font, std::string_view text_, SDL_Color color)
// {
//     SDL_Surface *surf{TTF_RenderText_Solid(font, text_.data(), color)};
//     if (surf == nullptr)
//     {
//         std::cout << "Couldn't load text \"" << text_ << "\".\n\tError: " <<
//         TTF_GetError() << std::flush; return nullptr;
//     }

//     SDL_Texture *tex{SDL_CreateTextureFromSurface(win->getRenderer(), surf)};
//     if (tex == nullptr)
//     {
//         std::cout << "Couldn't turn \"" << text_ << "\" into a
//         texture.\n\tError: " << SDL_GetError() << std::flush; return nullptr;
//     }

//     SDL_FreeSurface(surf);

//     return tex;
// }

void tnt::AssetManager::AddImage(Window const* win, std::string_view image)
{
    if (auto it{images.find(image.data())}; it != images.end() && it->second != nullptr)
        return;
    images[image.data()] = IMG_LoadTexture(win->getRenderer(), image.data());
}

void tnt::AssetManager::AddFont(std::string_view font, int size)
{
    if (auto it{fonts.find(font.data())}; it != fonts.end() && it->second != nullptr)
        return;
    fonts[font.data()] = TTF_OpenFont(font.data(), size);
}

// void tnt::AssetManager::AddText(Window const *win, std::string_view filename,
// std::string_view text, int size, SDL_Color const &color)
// {
//     if (auto it{this->text.find(text.data())}; it != this->text.end() &&
//     it->second != nullptr)
//         return;

//     if (auto it{fonts.find(filename.data())}; it == fonts.end() || it->second
//     == nullptr)
//         AddFont(filename, size);
//     auto font{Font(filename.data(), size)};
//     this->text[text.data()] = LoadText(win, font, text.data(), color);
// }

void tnt::AssetManager::AddMusic(std::string_view name)
{
    if (auto it{music.find(name.data())}; it != music.end() && it->second != nullptr)
        return;
    music[name.data()] = Mix_LoadMUS(name.data());
}

void tnt::AssetManager::AddSfx(std::string_view chunk)
{
    if (auto it{sfx.find(chunk.data())}; it != sfx.end() && it->second != nullptr)
        return;
    sfx[chunk.data()] = Mix_LoadWAV(chunk.data());
}

// void tnt::AssetManager::AddMap(std::string_view name)
// {
//     if (auto it{maps.find(name.data())}; it != maps.end() && it->second !=
//     nullptr)
//         return;
//     maps[name.data()]; // unfinished
// }

SDL_Texture* tnt::AssetManager::Image(Window const* win, std::string_view image)
{
    AddImage(win, image);
    return images[image.data()];
}

TTF_Font* tnt::AssetManager::Font(std::string_view font, int size)
{
    AddFont(font.data(), size);
    return fonts[font.data()];
}