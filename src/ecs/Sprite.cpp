// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs/Sprite.hpp"

#include "core/Window.hpp"
#include "fileIO/AssetManager.hpp"

tnt::SpriteComponent::SpriteComponent(Window const *win, std::string_view file)
    : clipped{false}, texture{AssetManager::This().Image(win, file)},
      clipRect{0, 0, 0, 0}
{
    SDL_QueryTexture(texture, nullptr, nullptr, (int *)(&clipRect.w), (int *)(&clipRect.h));
}

tnt::SpriteComponent::SpriteComponent(
    Window const *win, std::string_view file,
    Rectangle const &location)
    : clipped{true}, clipRect{location},
      texture{AssetManager::This().Image(win, file)} {}

tnt::SpriteComponent::~SpriteComponent() noexcept
{
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void tnt::SpriteComponent::Draw(Window const *win, Rectangle const &dest, float angle) noexcept
{
    SDL_RenderCopyEx(
        win->getRenderer(), texture,
        &static_cast<SDL_Rect>(clipRect), &static_cast<SDL_Rect>(dest),
        angle, nullptr, SDL_FLIP_NONE);
}

SDL_Texture *tnt::SpriteComponent::getTexture() const noexcept
{
    return texture;
}

void tnt::SpriteComponent::setTexture(Window const *win, std::string_view filename) noexcept
{
    if (clipped)
        clipped = false;
    SDL_DestroyTexture(texture);
    texture = AssetManager::This().Image(win, filename);
    SDL_QueryTexture(texture, nullptr, nullptr, (int *)(&clipRect.w), (int *)(&clipRect.h));
}

void tnt::SpriteComponent::setTexture(
    Window const *win, std::string_view filename,
    tnt::Rectangle const &location) noexcept
{
    if (!clipped)
        clipped = true;
    SDL_DestroyTexture(texture);
    texture = AssetManager::This().Image(win, filename);
    clipRect = location;
}

int tnt::SpriteComponent::getWidth() const noexcept { return (int)clipRect.w; }

int tnt::SpriteComponent::getHeight() const noexcept { return (int)clipRect.h; }

tnt::Sprite::Sprite(Window const *win, std::string_view filename, float angle) noexcept
    : sprite{add<SpriteComponent>(win, filename)} {}

tnt::Sprite::Sprite(Window const *win, std::string_view filename,
                    Rectangle const &area, float angle) noexcept
    : sprite{add<SpriteComponent>(win, filename, area)} {}

tnt::Sprite::~Sprite() noexcept
{
    delete sprite;
    sprite = nullptr;
}

tnt::SpriteComponent *tnt::Sprite::getSprite() const noexcept { return sprite; }

void tnt::Sprite::Draw(tnt::Window const *win) noexcept
{
    Vector const pos{getPosition()};
    Vector const scale{getScale()};
    Rectangle const dest{
        pos.x - sprite->getWidth() * scale.x * .5f,
        pos.y - sprite->getHeight() * scale.y * .5f,
        sprite->getWidth() * scale.x, sprite->getHeight() * scale.y};

    sprite->Draw(win, dest, getAngle());
}

int tnt::Sprite::getWidth() const noexcept { return sprite->getWidth() * (int)scale.x; }
int tnt::Sprite::getHeight() const noexcept { return sprite->getHeight() * (int)scale.y; }