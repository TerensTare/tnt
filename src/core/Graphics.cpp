// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "core/Graphics.hpp"

void tnt::drawLine(tnt::Window const &win, tnt::Vector const &start, tnt::Vector const &finish, SDL_Color const color, unsigned const thickness)
{
    Uint8 rgba[4]{0, 0, 0, 0};
    SDL_GetRenderDrawColor(win.getRenderer(), rgba, rgba + 1, rgba + 2, rgba + 3);

    SDL_SetRenderDrawColor(win.getRenderer(), color.r, color.g, color.b, color.a);

    for (unsigned i{0}; i < thickness; ++i)
        SDL_RenderDrawLineF(win.getRenderer(), start.x, start.y + i, finish.x, finish.y + i);

    SDL_SetRenderDrawColor(win.getRenderer(), rgba[0], rgba[1], rgba[2], rgba[3]);
}

void tnt::drawHorizontalLine(Window const &win, float const x1, float const x2, float const y,
                             SDL_Color const color, unsigned const thickness)
{
    Uint8 rgba[4]{0, 0, 0, 0};
    SDL_GetRenderDrawColor(win.getRenderer(), rgba, rgba + 1, rgba + 2, rgba + 3);

    SDL_SetRenderDrawColor(win.getRenderer(), color.r, color.g, color.b, color.a);

    for (unsigned i{0}; i < thickness; ++i)
        SDL_RenderDrawLineF(win.getRenderer(), x1, y + i, x2, y + i);

    SDL_SetRenderDrawColor(win.getRenderer(), rgba[0], rgba[1], rgba[2], rgba[3]);
}

void tnt::drawVerticalLine(Window const &win, float const x, float const y1, float const y2,
                           SDL_Color const color, unsigned const thickness)
{
    Uint8 rgba[4]{0, 0, 0, 0};
    SDL_GetRenderDrawColor(win.getRenderer(), rgba, rgba + 1, rgba + 2, rgba + 3);

    SDL_SetRenderDrawColor(win.getRenderer(), color.r, color.g, color.b, color.a);

    for (unsigned i{0}; i < thickness; ++i)
        SDL_RenderDrawLineF(win.getRenderer(), x, y1 + i, x, y2 + i);

    SDL_SetRenderDrawColor(win.getRenderer(), rgba[0], rgba[1], rgba[2], rgba[3]);
}