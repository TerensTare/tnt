#ifndef TNT_SDL_UTILS_HPP
#define TNT_SDL_UTILS_HPP

#include <memory>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

namespace std
{
template <>
struct default_delete<SDL_Window>
{
    void operator()(SDL_Window *_Ptr) const noexcept
    {
        SDL_DestroyWindow(_Ptr);
    }
};

template <>
struct default_delete<SDL_Renderer>
{
    void operator()(SDL_Renderer *_Ptr) const noexcept
    {
        SDL_DestroyRenderer(_Ptr);
    }
};

template <>
struct default_delete<SDL_Texture>
{
    void operator()(SDL_Texture *_Ptr) const noexcept
    {
        SDL_DestroyTexture(_Ptr);
    }
};

template <>
struct default_delete<SDL_Surface>
{
    void operator()(SDL_Surface *_Ptr) const noexcept
    {
        SDL_FreeSurface(_Ptr);
    }
};

template <>
struct default_delete<TTF_Font>
{
    void operator()(TTF_Font *_Ptr) const noexcept
    {
        TTF_CloseFont(_Ptr);
    }
};

template <>
struct default_delete<Mix_Chunk>
{
    void operator()(Mix_Chunk *_Ptr) const noexcept
    {
        Mix_FreeChunk(_Ptr);
    }
};

template <>
struct default_delete<Mix_Music>
{
    void operator()(Mix_Music *_Ptr) const noexcept
    {
        Mix_FreeMusic(_Ptr);
    }
};
} // namespace std

namespace tnt
{
} // namespace tnt

#endif //!TNT_SDL_UTILS_HPP