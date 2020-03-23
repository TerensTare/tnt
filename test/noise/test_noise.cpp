#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "pcg/Noise.hpp"
#include "utils/Logger.hpp"

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        tnt::logger::debug("Couldn't init SDL2!\nError: {}", SDL_GetError());
        return -1;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        tnt::logger::debug("Couldn't init SDL_Image!\nError: {}", IMG_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Window *win{SDL_CreateWindow("noise test", 100, 100, 800, 600, SDL_WINDOW_HIDDEN)};
    if (!win)
    {
        tnt::logger::debug("window error: {}", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *ren{SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED)};
    if (!ren)
    {
        tnt::logger::debug("renderer error: {}", SDL_GetError());

        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_Texture *tex{tnt::pcg::staticNoise(ren, 800, 600)};
    if (tex != nullptr)
    {
        Uint32 *pixels;
        SDL_SetRenderTarget(ren, tex);

        SDL_Surface *surf{SDL_CreateRGBSurface(0, 800, 600, 32, rmask, gmask, bmask, amask)};
        SDL_RenderReadPixels(ren, nullptr, surf->format->format, surf->pixels, surf->pitch);

        IMG_SavePNG(surf, "test.png");
        SDL_FreeSurface(surf);
    }
    else
    {
        SDL_DestroyRenderer(ren);
        ren = nullptr;

        SDL_DestroyWindow(win);
        win = nullptr;

        IMG_Quit();
        SDL_Quit();

        return 0;
    }

    bool running{true};
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                running = false;

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        SDL_Rect rct{SDL_Rect{0, 0, 800, 600}};
        SDL_RenderCopy(ren, tex, nullptr, &rct);
        SDL_RenderClear(ren);
        SDL_RenderPresent(ren);
    }

    SDL_DestroyTexture(tex);
    tex = nullptr;

    SDL_DestroyRenderer(ren);
    ren = nullptr;

    SDL_DestroyWindow(win);
    win = nullptr;

    IMG_Quit();
    SDL_Quit();

    return 0;
}