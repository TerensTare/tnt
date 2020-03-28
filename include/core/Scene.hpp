#ifndef TNT_SCENE_HPP
#define TNT_SCENE_HPP

#include "Camera.hpp"
#include "fileIO/AssetManager.hpp"
#include "core/Space.hpp"

// TODO: implement cutscenes (scenes where player input is ignored).
// TODO: store a Quadtree on each Scene (or Space).
// TODO: add more functionality.

// TODO(maybe):
// this should be an interface.

namespace tnt
{
class Scene
{
public:
    Scene();
    ~Scene() noexcept;

    virtual void LoadAssets() = 0;
    void Draw();
    void Update();

private:
    Camera *camera;
    std::weak_ptr<Scene> scenes; // TODO(maybe): THIS should be moved into game
    AssetManager &assets;
};
} // namespace tnt

#endif //!TNT_SCENE_HPP