// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs2/Drawable.hpp"

#include <SDL2/SDL_render.h>

#include "core/Window.hpp"
#include "fileIO/AssetManager.hpp"

tnt::ecs2::SpriteComp::SpriteComp(Window const *win, std::string_view file)
    : clipped{false}, texture{AssetManager::This().Image(win, file)},
      clipRect{0, 0, 0, 0}
{
    SDL_QueryTexture(texture, nullptr, nullptr, (int *)(&clipRect.w), (int *)(&clipRect.h));
}

tnt::ecs2::SpriteComp::SpriteComp(
    Window const *win, std::string_view file,
    Rectangle const &location)
    : clipped{true}, clipRect{location},
      texture{AssetManager::This().Image(win, file)} {}

tnt::ecs2::SpriteComp::~SpriteComp() noexcept
{
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

SDL_Texture *tnt::ecs2::SpriteComp::getTexture() const noexcept
{
    return texture;
}

void tnt::ecs2::SpriteComp::setTexture(Window const *win, std::string_view filename) noexcept
{
    if (clipped)
        clipped = false;
    SDL_DestroyTexture(texture);
    texture = AssetManager::This().Image(win, filename);
    SDL_QueryTexture(texture, nullptr, nullptr, (int *)(&clipRect.w), (int *)(&clipRect.h));
}

void tnt::ecs2::SpriteComp::setTexture(
    Window const *win, std::string_view filename,
    tnt::Rectangle const &location) noexcept
{
    if (!clipped)
        clipped = true;
    SDL_DestroyTexture(texture);
    texture = AssetManager::This().Image(win, filename);
    clipRect = location;
}

void tnt::ecs2::Sprite::Draw(tnt::Window const *win) noexcept
{
    Rectangle const dest{
        pos.x - clipRect.x * scale.x * .5f,
        pos.y - clipRect.y * scale.y * .5f,
        clipRect.w * scale.x, clipRect.h * scale.y};

    SDL_RenderCopyEx(
        win->getRenderer(), texture,
        &static_cast<SDL_Rect>(clipRect), &static_cast<SDL_Rect>(dest),
        angle, nullptr, SDL_FLIP_NONE);
}