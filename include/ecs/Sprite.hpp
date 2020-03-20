#ifndef TNT_SPRITE_HPP
#define TNT_SPRITE_HPP

#include "Renderable.hpp"
#include "ecs/Object.hpp"

namespace tnt
{
struct Sprite
    : public Object,
      public Renderable
{
    Sprite(std::string_view filename);
    ~Sprite() noexcept;
};
} // namespace tnt

#endif //!TNT_SPRITE_HPP