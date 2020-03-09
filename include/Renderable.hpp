#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include "utils/SDL_Utils.hpp"

namespace tnt
{
class __declspec(novtable) Renderable
{
public:
    virtual ~Renderable() noexcept;

    virtual void Draw() = 0;

protected:
    std::weak_ptr<SDL_Texture> tex;
};
} // namespace tnt

#endif //!RENDERABLE_HPP