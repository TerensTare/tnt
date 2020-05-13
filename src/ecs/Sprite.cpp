// This is an independent project of an individual developer. Dear
// PVS-Studio, please check it. PVS-Studio Static Code Analyzer for C,
// C++, C#, and Java: http://www.viva64.com

#include "ecs/Sprite.hpp"

#include "core/Window.hpp"
#include "utils/Timer.hpp"

tnt::Sprite::Sprite(Window const *win, std::string_view filename, float angle)
    : sprite{add<SpriteComponent>(win, filename)} {}

tnt::Sprite::Sprite(Window const *win, std::string_view filename,
                    Rectangle const &area, float angle)
    : sprite{add<SpriteComponent>(win, filename, area)} {}

tnt::Sprite::~Sprite() noexcept
{
  delete sprite;
  sprite = nullptr;
}

tnt::SpriteComponent *tnt::Sprite::getSprite() const noexcept { return sprite; }
