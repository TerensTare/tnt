#ifndef TNT_OBJECT_HPP
#define TNT_OBJECT_HPP

#include <memory>
#include <unordered_set>
#include "ecs/Component.hpp"

// TODO:
// Serializable interface class.
// Add Serialization to Object and it's childrens.
// handle global and local coordinates.
// TODO(maybe):
// Move this to core.

namespace tnt
{
class Object
{
public:
    virtual ~Object() noexcept;

    void setParent(std::shared_ptr<Object> obj) noexcept;
    std::shared_ptr<Object> getParent() const noexcept;

    void addComponent(Component &&component) noexcept;
    void removeComponent(Component &&comp) noexcept;

    void setPosition(Vector const &pos) noexcept;
    Vector getPosition() const noexcept;

    void Transform(Vector const &pos) noexcept;

    virtual void Update() = 0;

protected:
    Vector position;
    std::vector<Component *> components;
    std::weak_ptr<Object> parent;
};
} // namespace tnt

#endif //!TNT_OBJECT_HPP