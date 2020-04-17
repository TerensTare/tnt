#ifndef TNT_SPRITE_HPP
#define TNT_SPRITE_HPP

#include <map>

#include "ecs/Component.hpp"
#include "ecs/Object.hpp"

// TODO: setIdleAnimation ??

// TODO(maybe):
// addAnimation ??
// move this to the gui folder ??

namespace tnt
{
    class Window;
    class Timer;

    class Sprite : virtual public Object
    {
      public:
        Sprite(Window const *win, std::string_view filename,
               float angle = 0.f);
        Sprite(Window const *win, std::string_view filename,
               Rectangle const &area, float angle = 0.f);

        virtual ~Sprite() noexcept;

        RotateComponent *getRotate() const noexcept;
        ScaleComponent *getScale() const noexcept;
        SpriteComponent *getSprite() const noexcept;

        void addAnimation(Window const *win, std::string_view name,
                          int framesNum, float speed, bool horizontal,
                          Rectangle const &clip) noexcept;
        void playAnimation(std::string_view animation) noexcept;

      protected:
        RotateComponent *rotate;
        ScaleComponent *scale;
        SpriteComponent *sprite;
        std::string currentAnim;
        std::string defaultAnim;
        Timer *timer;
        std::map<std::string, AnimationComponent *> animations;
    };
} // namespace tnt

#endif //! TNT_SPRITE_HPP