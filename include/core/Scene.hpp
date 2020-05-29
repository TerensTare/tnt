#ifndef TNT_SCENE_HPP
#define TNT_SCENE_HPP

#include <map>
#include <span>
#include "core/Camera.hpp"

// TODO: implement cutscenes (scenes where player input is ignored).
// TODO: store a Quadtree on each Scene (or Space).
// TODO: add more functionality.
// TODO: move loader functions on class Game.

// TODO(maybe):
// make LoadAssets() a coroutine ??
// use std::span on LoadAssets ??

typedef struct SDL_Texture SDL_Texture;

namespace tnt
{
    class Window;
    class Space;

    /// @brief A class that handles different @c Spaces.
    class Scene
    {
    public:
        explicit Scene(Window const *win, std::string_view background = "") noexcept;

        // NOTE: fonts are loaded using LoadFont().
        void LoadAssets(Window const *win, std::span<std::string_view> assets);
        void LoadFont(std::string_view name, int size);

        void addSpace(std::string_view name, Space *space);
        Space *getSpace(std::string_view name) const noexcept;

        void Draw(tnt::Window const *win);
        void Update(long long time_);

    private:
        SDL_Texture *bg;
        Camera camera_; // TODO: replace this with a concept if possible
        std::map<std::string, Space *> spaces;
    };
} // namespace tnt

#endif //! TNT_SCENE_HPP