// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "core/Graphics.hpp"

void tnt::drawLine(tnt::Window const &win, tnt::Vector const &start, tnt::Vector const &finish, SDL_Color const &color, unsigned const thickness) noexcept
{
    SDL_Renderer *ren = win.getRenderer();
    Uint8 rgba[4]{0, 0, 0, 0};
    SDL_GetRenderDrawColor(ren, rgba, rgba + 1, rgba + 2, rgba + 3);

    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);

    for (unsigned i{0}; i < thickness; ++i)
        SDL_RenderDrawLineF(ren, start.x, start.y + i, finish.x, finish.y + i);

    SDL_SetRenderDrawColor(ren, rgba[0], rgba[1], rgba[2], rgba[3]);
}

void tnt::drawHorizontalLine(Window const &win, float const x1, float const x2, float const y,
                             SDL_Color const &color, unsigned const thickness) noexcept
{
    SDL_Renderer *ren = win.getRenderer();
    Uint8 rgba[4]{0, 0, 0, 0};
    SDL_GetRenderDrawColor(ren, rgba, rgba + 1, rgba + 2, rgba + 3);

    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);

    for (unsigned i{0}; i < thickness; ++i)
        SDL_RenderDrawLineF(ren, x1, y + i, x2, y + i);

    SDL_SetRenderDrawColor(ren, rgba[0], rgba[1], rgba[2], rgba[3]);
}

void tnt::drawVerticalLine(Window const &win, float const x, float const y1, float const y2,
                           SDL_Color const &color, unsigned const thickness) noexcept
{
    SDL_Renderer *ren = win.getRenderer();
    Uint8 rgba[4]{0, 0, 0, 0};
    SDL_GetRenderDrawColor(ren, rgba, rgba + 1, rgba + 2, rgba + 3);

    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);

    for (unsigned i{0}; i < thickness; ++i)
        SDL_RenderDrawLineF(ren, x, y1 + i, x, y2 + i);

    SDL_SetRenderDrawColor(ren, rgba[0], rgba[1], rgba[2], rgba[3]);
}

void tnt::drawCircle(Window const &win, tnt::Vector const &topleft,
                     float const radius, SDL_Color const &color,
                     unsigned const thickness) noexcept
{
    SDL_Renderer *ren = win.getRenderer();
    Uint8 rgba[4]{0, 0, 0, 0};
    SDL_GetRenderDrawColor(ren, rgba, rgba + 1, rgba + 2, rgba + 3);
    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);

    for (float y{-radius}; y <= radius; y += 1.f)
        for (float x{-radius}; x <= radius; x += 1.f)
            if (fabsf(x * x + y * y - radius * radius) < 2 * thickness)
                SDL_RenderDrawPointF(ren, topleft.x + x, topleft.y + y);

    SDL_SetRenderDrawColor(win.getRenderer(), rgba[0], rgba[1], rgba[2], rgba[3]);
}

void tnt::fillCircle(Window const &win, tnt::Vector const &topleft,
                     float const radius, SDL_Color const &color) noexcept
{
    SDL_Renderer *ren = win.getRenderer();
    Uint8 rgba[4]{0, 0, 0, 0};
    SDL_GetRenderDrawColor(ren, rgba, rgba + 1, rgba + 2, rgba + 3);
    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);

    for (float y{-radius}; y < radius; y += 1.f)
        for (float x{-radius}; x < radius; x += 1.f)
            if (x * x + y * y < radius * radius)
                SDL_RenderDrawPointF(ren, topleft.x + x, topleft.y + y);

    SDL_SetRenderDrawColor(win.getRenderer(), rgba[0], rgba[1], rgba[2], rgba[3]);
}