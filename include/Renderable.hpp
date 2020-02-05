#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <SDL2/SDL_render.h>

namespace tnt
{
class Renderable
{
public:
    virtual ~Renderable() noexcept { SDL_DestroyTexture(tex); }

    virtual void Draw() = 0;

protected:
    SDL_Texture *tex;
};
} // namespace tnt

#endif //!RENDERABLE_HPP