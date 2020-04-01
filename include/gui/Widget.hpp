#ifndef WIDGET_HPP
#define WIDGET_HPP

// TODO: add operator| support.
// That should set widget on the right as the child of the widget on the left.
// TODO: Make widget a concept.

#include <string_view>
#include <SDL2/SDL_render.h>

#include "ecs/Object.hpp"
#include "ecs/Component.hpp"
#include "utils/Observer.hpp"

namespace tnt
{
class Window;

class Widget
    : public Observer,
      virtual public Object
{
public:
    virtual ~Widget() noexcept {}

    bool Active() const noexcept;

    // virtual void Update(Observable *obj) override;
    virtual void Draw() = 0;

protected:
    bool active;
    bool draggable;
    Rectangle area;
};

class Draggable : virtual public Widget
{
};

class Text : virtual public Widget
{
public:
    Text(std::string_view, int x, int y, int w = 14, int h = 14);
    ~Text() noexcept;

    virtual void Update(long long elapsed) override;
    virtual void Draw(Window const *target);

protected:
    int tX;
    int tY;
    int tW;
    int tH;
};
} // namespace tnt

#endif //!WIDGET_HPP