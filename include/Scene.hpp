#ifndef SCENE_HPP
#define SCENE_HPP

#include "AssetManager.hpp"

// TODO(maybe): this should be an interface.
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
    std::weak_ptr<Scene> scenes; // TODO(maybe): THIS should be moved into game
    AssetManager *assets;
};
} // namespace tnt

#endif //!SCENE_HPP