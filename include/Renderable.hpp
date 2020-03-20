#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include "utils/SDL_Utils.hpp"

// TODO(maybe):
// make this a class derived from Object.

namespace tnt
{
class __declspec(novtable) Renderable
{
public:
    virtual ~Renderable() noexcept;

    virtual void Draw() = 0;

protected:
    std::shared_ptr<SDL_Texture> tex;

    friend class Window;
};
} // namespace tnt

#endif //!RENDERABLE_HPP