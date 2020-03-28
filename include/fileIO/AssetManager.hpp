#ifndef TNT_ASSET_MANAGER_HPP
#define TNT_ASSET_MANAGER_HPP

#include <map>
#include <future>

#include "core/Window.hpp"

typedef struct _Mix_Music Mix_Music;

typedef struct Mix_Chunk Mix_Chunk;

typedef struct _TTF_Font TTF_Font;

namespace tnt
{
// TODO(maybe)(x): make this a non-singleton. ??
// TODO(maybe): add an animations map. ??
// TODO(maybe): remove friend class AudioManager; ??
// TODO(maybe): use std::unordered_map instead ??

// TODO: add better key value for text SDL_Texture*-s.
// TODO: getters aren't checking if asset has been loaded.
// TODO: make everything here asynchronous.
class AssetManager
{
public:
    ~AssetManager() noexcept;

    static AssetManager &This();

    void AddFont(std::string_view font, int size);
    void AddImage(SDL_Renderer *ren, std::string_view image);
    void AddText(SDL_Renderer *ren, std::string_view filename, std::string_view text, int size, SDL_Color const &color);
    void AddMusic(std::string_view music);
    void AddSfx(std::string_view chunk);

    std::future<SDL_Texture *> Image(std::string_view image);
    std::future<SDL_Texture *> Text(std::string_view txt);
    std::future<TTF_Font *> Font(std::string_view font, int size);
    std::future<Mix_Music *> Music(std::string_view name);
    std::future<Mix_Chunk *> Sfx(std::string_view chunk);

private:
    AssetManager();

    SDL_Texture *LoadImage(SDL_Renderer *ren, std::string filename);
    SDL_Texture *LoadText(SDL_Renderer *ren, TTF_Font *font, std::string text, SDL_Color color);

    std::recursive_mutex mtx;
    std::map<std::string, SDL_Texture *> images;
    std::map<std::string, SDL_Texture *> text;
    std::map<std::string, TTF_Font *> fonts;
    std::map<std::string, Mix_Music *> music;
    std::map<std::string, Mix_Chunk *> sfx;

    friend class AudioPlayer;
};
} // namespace tnt

#endif //!TNT_ASSET_MANAGER_HPP