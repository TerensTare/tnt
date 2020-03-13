#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

struct SDL_Renderer;
typedef struct SDL_Renderer SDL_Renderer;

// TODO:
// review the whole rendering process and infrastructure.

namespace tnt
{
class Graphics
{
public:
    static Graphics &This();

private:
    Graphics();
    ~Graphics() noexcept;

    static bool Init();
    SDL_Renderer *getRenderer() const noexcept;

    static bool init;
    SDL_Renderer *ren;

    friend class Window;
};
} // namespace tnt

#endif //!GRAPHICS_HPP