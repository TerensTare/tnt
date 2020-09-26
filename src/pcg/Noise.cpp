// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <span>
#include <SDL2/SDL_render.h>

#include "pcg/Noise.hpp"
#include "pcg/Random.hpp"

#include "utils/Logger.hpp"
#include "utils/CoUtils.hpp"

SDL_Texture *tnt::pcg::staticNoise(SDL_Renderer *ren, int w, int h)
{
    SDL_PixelFormat pixelFmt;
    SDL_Texture *ret = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    SDL_QueryTexture(ret, &pixelFmt.format, nullptr, nullptr, nullptr);

    std::vector<Uint32> pixels;
    pixels.reserve(w * h * 4);
    int pitch{0};
    if (SDL_LockTexture(ret, nullptr, (void **)pixels.data(), &pitch) < 0)
    {
        logger::debug("staticNoise couldn't lock texture!!\nError: {}", SDL_GetError());
        return nullptr;
    }

    for (int const &i : range(0, h))
        for (int const &j : range(0, w))
        {
            const uint8_t &r{static_cast<Uint8>(randomInt(0, 255))};
            const uint8_t &g{static_cast<Uint8>(randomInt(0, 255))};
            const uint8_t &b{static_cast<Uint8>(randomInt(0, 255))};

            const Uint32 &color{SDL_MapRGB(&pixelFmt, r, g, b)};
            const Uint32 pos{i * (pitch / sizeof(unsigned)) + j};
            pixels[pos] = color;
        }

    SDL_UnlockTexture(ret);
    return ret;
}
