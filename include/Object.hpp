#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Vector.hpp"

namespace tnt
{
class Object
{
public:
    ~Object() noexcept;

    void setAngle(float rad) noexcept;
    float getAngle() const noexcept;

    void setScale(Vector s) noexcept;
    Vector getScale() const noexcept;

    void setPosition(Vector p) noexcept;
    Vector getPosition() const noexcept;

    void setParent(Object *obj) noexcept;
    Object *getParent() const noexcept;

    void Rotate(float rad) noexcept;
    void Scale(Vector s) noexcept;
    void Transform(Vector p) noexcept;

    virtual void Update() = 0;

protected:
    float angle;
    Vector scale;
    Vector pos;
    Object *parent;
};
} // namespace tnt

#endif //!OBJECT_HPP