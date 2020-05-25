#ifndef TNT_SCENE_HPP
#define TNT_SCENE_HPP

#include <map>
#include "core/Camera.hpp"

// TODO: implement cutscenes (scenes where player input is ignored).
// TODO: store a Quadtree on each Scene (or Space).
// TODO: add more functionality.
// TODO: move loader functions on class Game.

// TODO(maybe):
// make LoadAssets() a coroutine ??

namespace tnt
{
    class Window;
    class Space;

    /// @brief A class that handles different @c Spaces.
    class Scene
    {
    public:
        explicit Scene(std::shared_ptr<Window> const &window_) noexcept;
        ~Scene() noexcept;

        // NOTE: fonts are loaded using LoadFont().
        void LoadAssets(std::vector<std::string> const &assets);
        void LoadFont(std::string_view name, int size);

        void addSpace(std::string_view name, Space *space);

        void Draw();
        void Update(long long time_);

    private:
        Camera *camera_; // TODO: replace this with a concept if possible
        std::weak_ptr<Window> window;
        std::vector<Space *> spaces;
    };
} // namespace tnt

#endif //! TNT_SCENE_HPP