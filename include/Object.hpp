#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <memory>
#include "math/Vector.hpp"

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

    void setAngle(float rad) noexcept;
    float getAngle() const noexcept;

    void setScale(Vector const &s) noexcept;
    Vector getScale() const noexcept;

    void setPosition(Vector const &p) noexcept;
    Vector getPosition() const noexcept;

    void setParent(std::shared_ptr<Object> obj) noexcept;
    std::shared_ptr<Object> getParent() const noexcept;

    void Rotate(float rad) noexcept;
    void Scale(Vector const &s) noexcept;
    void Transform(Vector const &p) noexcept;

    virtual void Update() = 0;

protected:
    float angle;
    Vector scale;
    Vector pos;
    std::weak_ptr<Object> parent;
};
} // namespace tnt

#endif //!OBJECT_HPP