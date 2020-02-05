#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <map>
#include <future>
#include <iostream>

#include <sol/sol.hpp>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

namespace tnt
{

// TODO(maybe)(x): make this a non-singleton.
// TODO(maybe): add an animations map.
// TODO(maybe): move modules to LuaManager.
// TODO(maybe): remove friend class AudioManager;

// TODO: add better key value for text SDL_Texture*-s.
// TODO: getters aren't checking if asset has been loaded.
class AssetManager
{
public:
    AssetManager();
    ~AssetManager() noexcept;

    std::future<void> AddFont(std::string const &font, int size);
    std::future<void> AddImage(SDL_Renderer *ren, std::string const &image);
    std::future<void> AddText(SDL_Renderer *ren, std::string const &filename, std::string const &text, int size, SDL_Color const &color);
    std::future<void> AddMusic(std::string const &music);
    std::future<void> AddSfx(std::string const &chunk);
    std::future<void> AddModule(std::string const &module);

    std::future<SDL_Texture *> Image(std::string const &image);
    std::future<SDL_Texture *> Text(std::string const &txt);
    std::future<TTF_Font *> Font(std::string const &font, int size);
    std::future<Mix_Music *> Music(std::string const &name);
    std::future<Mix_Chunk *> Sfx(std::string const &chunk);
    std::future<sol::table> Module(std::string const &module);

private:
    SDL_Texture *LoadImage(SDL_Renderer *ren, std::string filename);
    SDL_Texture *LoadText(SDL_Renderer *ren, TTF_Font *font, std::string text, SDL_Color color);

    std::map<std::string, SDL_Texture *> images;
    std::map<std::string, SDL_Texture *> text;
    std::map<std::string, TTF_Font *> fonts;
    std::map<std::string, Mix_Music *> music;
    std::map<std::string, Mix_Chunk *> sfx;
    std::map<std::string, sol::table> modules;

    friend class AudioPlayer;
};
} // namespace tnt

#endif //!ASSET_MANAGER_HPP