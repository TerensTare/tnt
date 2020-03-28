// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs/Sprite.hpp"

tnt::Sprite::Sprite(Window const *win, std::string_view filename, float angle)
    : sprite{new SpriteComponent{win, filename}},
      rotate{new RotateComponent{angle}},
      scale{new ScaleComponent{1.f, 1.f}}
{
}

tnt::Sprite::~Sprite() noexcept
{
    delete rotate;
    rotate = nullptr;

    delete scale;
    scale = nullptr;

    delete sprite;
    sprite = nullptr;
}

tnt::RotateComponent *tnt::Sprite::getRotate() const noexcept { return rotate; }
tnt::ScaleComponent *tnt::Sprite::getScale() const noexcept { return scale; }
tnt::SpriteComponent *tnt::Sprite::getSprite() const noexcept { return sprite; }