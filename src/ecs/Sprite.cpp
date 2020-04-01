// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs/Sprite.hpp"
#include "core/Window.hpp"

tnt::Sprite::Sprite(Window const *win, std::string_view filename, float angle)
    : sprite{new SpriteComponent{win, filename}},
      rotate{new RotateComponent{angle}},
      scale{new ScaleComponent{VECTOR_ONE}} {}

tnt::Sprite::Sprite(Window const *win, std::string_view filename,
                    Rectangle const &area, float angle)
    : sprite{new SpriteComponent{win, filename, area}},
      rotate{new RotateComponent{angle}},
      scale{new ScaleComponent{VECTOR_ONE}} {}

tnt::Sprite::~Sprite() noexcept
{
  delete sprite;
  sprite = nullptr;

  delete rotate;
  rotate = nullptr;

  delete scale;
  scale = nullptr;
}

tnt::RotateComponent *tnt::Sprite::getRotate() const noexcept { return rotate; }
tnt::ScaleComponent *tnt::Sprite::getScale() const noexcept { return scale; }
tnt::SpriteComponent *tnt::Sprite::getSprite() const noexcept { return sprite; }