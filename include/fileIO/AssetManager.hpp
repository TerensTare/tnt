#ifndef TNT_ASSET_MANAGER_HPP
#define TNT_ASSET_MANAGER_HPP

#include <map>
#include <SDL2/SDL_mixer.h>

typedef struct _TTF_Font TTF_Font;

namespace tnt
{
class Window;

// TODO(maybe)(x): make this a non-singleton. ??
// TODO(maybe): add an animations map. ??
// TODO(maybe): remove friend class AudioManager; ??
// TODO(maybe): use std::unordered_map instead ??

// TODO: add better key value for text SDL_Texture*-s.
// TODO: getters aren't checking if asset has been loaded.
// TODO: make everything here asynchronous.
// TODO: store TileMap datas here.
class AssetManager
{
public:
    ~AssetManager() noexcept;

    static AssetManager &This();

    void AddFont(std::string_view font, int size);
    void AddImage(Window const *win, std::string_view image);
    void AddText(Window const *win, std::string_view filename, std::string_view text, int size, SDL_Color const &color);
    void AddMusic(std::string_view music);
    void AddSfx(std::string_view chunk);

    SDL_Texture *Image(Window const *win, std::string_view image);
    SDL_Texture *Text(Window const *win, std::string_view txt);
    TTF_Font *Font(std::string_view font, int size);
    Mix_Music *Music(std::string_view name);
    Mix_Chunk *Sfx(std::string_view chunk);

private:
    AssetManager();

    SDL_Texture *LoadText(Window const *win, TTF_Font *font, std::string_view text, SDL_Color color);

    std::map<std::string, SDL_Texture *> images;
    std::map<std::string, SDL_Texture *> text;
    std::map<std::string, TTF_Font *> fonts;
    std::map<std::string, Mix_Music *> music;
    std::map<std::string, Mix_Chunk *> sfx;

    // friend class AudioPlayer;
};
} // namespace tnt

#endif //!TNT_ASSET_MANAGER_HPP