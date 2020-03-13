// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "core/Graphics.hpp"
#include "utils/Logger.hpp"

bool tnt::Graphics::init = false;

tnt::Graphics::Graphics()
{
    init = Init();
}

tnt::Graphics::~Graphics() noexcept
{
    SDL_DestroyRenderer(ren);
    ren = nullptr;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

tnt::Graphics &tnt::Graphics::This()
{
    if (init)
    {
        static Graphics inst;
        return inst;
    }
    init = Init();
    return This();
}

bool tnt::Graphics::Init()
{
    if (!init)
    {
        if (auto res{SDL_Init(SDL_INIT_EVERYTHING)}; res < 0)
        {
            tnt::logger::error("Couldn't initalize SDL2!! Error: {}\n", SDL_GetError());
            return false;
        }

#define flags (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP)

        if (auto res{IMG_Init(flags)};
            res & flags != flags)
        {
            tnt::logger::error("Couldn't initialize SDL_Image!! Error: {}\n", IMG_GetError());
            return false;
        }

        if (auto res{TTF_Init()}; res == -1)
        {
            tnt::logger::error("Couldn't initalize SDL_ttf!! Error: {}\n", TTF_GetError());
            return false;
        }

        tnt::logger::debug("Successfully initialized all graphics systems.");
    }

    return true;
}

SDL_Renderer *tnt::Graphics::getRenderer() const noexcept { return ren; }