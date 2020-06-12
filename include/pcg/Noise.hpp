#ifndef TNT_PCG_NOISE_HPP
#define TNT_PCG_NOISE_HPP

struct SDL_Renderer;
typedef struct SDL_Renderer SDL_Renderer;

struct SDL_Texture;
typedef struct SDL_Texture SDL_Texture;

// TODO: add more noise generation algorithms.
// TODO: add maze generation algorithms.

namespace tnt::pcg
{
    /// @brief Create a texture with some random noise on it.
    /// @param ren The renderer where the texture should be drawed.
    /// @param w The width of the texture.
    /// @param h The height of the texture.
    SDL_Texture *staticNoise(SDL_Renderer *ren, int w, int h);
} // namespace tnt::pcg

#endif //! TNT_PCG_NOISE_HPP