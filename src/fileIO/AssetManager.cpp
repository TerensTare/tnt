// This is an independent project of an individual developer. Dear PVS-Studio,
// please check it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "fileIO/AssetManager.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// #include "tiled/TileMap.hpp"
#include "core/Window.hpp"

// TODO:
// use tnt::logger instead here.

tnt::AssetManager::AssetManager() {}

tnt::AssetManager::~AssetManager() noexcept
{
    if (!images.empty())
        for (auto const &it : images)
            if (it.second != nullptr)
                SDL_DestroyTexture(it.second);
    images.clear();

    // if (!text.empty())
    //     for (auto const &it : text)
    //         if (it.second != nullptr)
    //             SDL_DestroyTexture(it.second);
    // text.clear();

    if (!fonts.empty())
        for (auto const &it : fonts)
            if (it.second != nullptr)
                TTF_CloseFont(it.second);
    fonts.clear();

    if (!music.empty())
        for (auto const &it : music)
            if (it.second != nullptr)
                Mix_FreeMusic(it.second);
    music.clear();

    if (!sfx.empty())
        for (auto const &it : sfx)
            if (it.second != nullptr)
                Mix_FreeChunk(it.second);
    sfx.clear();

    // for (auto const&it : maps)
    //     if (it.second != nullptr)
    //         maps.erase(it);
    // maps.clear();
    // std::map<std::string, tmx::TileMap *>{}.swap(maps);
}

tnt::AssetManager &tnt::AssetManager::This()
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
//
//     SDL_Texture *tex{SDL_CreateTextureFromSurface(win->getRenderer(), surf)};
//     if (tex == nullptr)
//     {
//         std::cout << "Couldn't turn \"" << text_ << "\" into a
//         texture.\n\tError: " << SDL_GetError() << std::flush; return nullptr;
//     }
//
//     SDL_FreeSurface(surf);
//
//     return tex;
// }

auto absolute = [](std::string_view path_) -> std::string {
    return std::string{SDL_GetBasePath()}.append(path_);
};

template <typename T>
using assets_it = typename std::map<std::string, T>::const_iterator;

void tnt::AssetManager::AddImage(Window const *win, std::string_view image)
{
    std::string image_{absolute(image)};
    if (assets_it<SDL_Texture *> it{images.find(image_)}; it != images.cend() && it->second != nullptr)
        return;
    images[image_] = IMG_LoadTexture(win->getRenderer(), image_.c_str());
}

void tnt::AssetManager::AddFont(std::string_view font, int size)
{
    std::string font_{absolute(font)};
    if (assets_it<TTF_Font *> it{fonts.find(font_)}; it != fonts.cend() && it->second != nullptr)
        return;
    fonts[font_] = TTF_OpenFont(font_.c_str(), size);
}

// void tnt::AssetManager::AddText(Window const *win, std::string_view filename,
// std::string_view text, int size, SDL_Color const &color)
// {
//     if (auto it{this->text.find(text.data())}; it != this->text.end() &&
//     it->second != nullptr)
//         return;
//
//     if (auto it{fonts.find(filename.data())}; it == fonts.end() || it->second
//     == nullptr)
//         AddFont(filename, size);
//     auto font{Font(filename.data(), size)};
//     this->text[text.data()] = LoadText(win, font, text.data(), color);
// }

void tnt::AssetManager::AddMusic(std::string_view name)
{
    std::string music_{absolute(name)};
    if (assets_it<Mix_Music *> it{music.find(music_)};
        it != music.cend() && it->second != nullptr)
        return;
    music[music_] = Mix_LoadMUS(music_.c_str());
}

void tnt::AssetManager::AddSfx(std::string_view chunk)
{
    std::string chunk_{absolute(chunk)};
    if (assets_it<Mix_Chunk *> it{sfx.find(chunk_)};
        it != sfx.cend() && it->second != nullptr)
        return;
    sfx[chunk_] = Mix_LoadWAV(chunk_.c_str());
}

// void tnt::AssetManager::AddMap(std::string_view name)
// {
//     if (auto it{maps.find(name.data())}; it != maps.end() && it->second !=
//     nullptr)
//         return;
//     maps[name.data()]; // unfinished
// }

SDL_Texture *tnt::AssetManager::Image(Window const *win, std::string_view image)
{
    AddImage(win, image);
    return images[absolute(image)];
}

TTF_Font *tnt::AssetManager::Font(std::string_view font, int size)
{
    AddFont(font.data(), size);
    return fonts[absolute(font)];
}

Mix_Music *tnt::AssetManager::Music(std::string_view name)
{
    AddMusic(name);
    return music[absolute(name)];
}

Mix_Chunk *tnt::AssetManager::Sfx(std::string_view chunk)
{
    AddSfx(chunk);
    return sfx[absolute(chunk)];
}