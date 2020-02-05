#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Window.hpp"

namespace tnt
{
// this class controlls all the "activities" of the engine.
class Engine
{
public:
    Engine();

private:
    Window *window;
};

} // namespace tnt

#endif //!ENGINE_HPP