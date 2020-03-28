#ifndef WIDGET_HPP
#define WIDGET_HPP

// TODO: add operator| support.
// That should set widget on the right as the child of the widget on the left.
// TODO: Make widget a concept.

#include <string_view>
#include <SDL2/SDL_render.h>

#include "core/Window.hpp"
#include "ecs/Object.hpp"
#include "utils/Observer.hpp"

namespace tnt
{
class Widget
    : public Observer,
      public Object
{
public:
    virtual ~Widget() noexcept {}

    bool Active() const noexcept;

    // virtual void Update() override;

protected:
    bool active;
    bool draggable;
    SpriteComponent *sprite;
};

class Draggable : public Widget
{
};

class Text : public Widget
{
public:
    Text(std::string_view, int x, int y, int w = 14, int h = 14);
    ~Text() noexcept;

    virtual void Update() override;
    virtual void Draw(Window const *target);

protected:
    int tX;
    int tY;
    int tW;
    int tH;
};
} // namespace tnt

#endif //!WIDGET_HPP