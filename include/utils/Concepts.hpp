#ifndef TNT_CONCEPTS_HPP
#define TNT_CONCEPTS_HPP

#include <memory>
#include <type_traits>

typedef struct SDL_FRect SDL_FRect;
typedef struct SDL_Rect SDL_Rect;

namespace tnt
{
class Component;

template <typename T>
concept reversable = requires(T t)
{
    t.rbegin();
    t.rend();
};

template <typename T>
concept observable = requires(T t)
{
    t.Attach();
    t.Detach();
    t.Notify();
};

template <typename T>
concept singleton = requires
{
    {
        T::This()
    }
    ->T &;
};

template <typename T>
concept renderable = requires(T t)
{
    t.tex->std::shared_ptr<SDL_Texture>;
};

template <typename T>
concept camera_type = requires(T t)
{
    {
        t.FBounds()
    }
    ->SDL_FRect;
    {
        t.Bounds()
    }
    ->SDL_Rect;
};

template <typename T>
concept component = std::is_base_of_v<Component, T>;
} // namespace tnt

#endif //!TNT_CONCEPTS_HPP