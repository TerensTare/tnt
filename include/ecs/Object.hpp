#ifndef TNT_OBJECT_HPP
#define TNT_OBJECT_HPP

#include <vector>
#include "math/Vector.hpp"

// TODO:
// Serializable interface class.
// Add Serialization to Object and it's childrens.
// handle global and local coordinates.

// TODO(maybe):
// StaticObject ??
// Serializable as a Component ??
namespace tnt
{
class Component;

class Object
{
public:
    virtual ~Object() noexcept;

    void setParent(std::shared_ptr<Object> obj) noexcept;
    Object *getParent() const noexcept;

    virtual void Update() = 0;

protected:
    std::weak_ptr<Object> parent;
};
} // namespace tnt

#endif //!TNT_OBJECT_HPP