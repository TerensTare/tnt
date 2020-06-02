#ifndef TNT_ECS2_TRANSFORMABLE_HPP
#define TNT_ECS2_TRANSFORMABLE_HPP

#include "ecs2/Object.hpp"

namespace tnt::ecs2
{
    class RotateComp : public Component
    {
    public:
        explicit RotateComp(float angle_) noexcept;

        void setAngle(float angle_) noexcept;
        float getAngle() const noexcept;

        void Rotate(float angle_) noexcept;

    protected:
        float angle;
    };

    class ScaleComp : public Component
    {
    public:
        explicit ScaleComp(Vector const &v) noexcept;
        ScaleComp(float x, float y) noexcept;

        void setScale(Vector const &v) noexcept;
        Vector getScale() const noexcept;

        void Scale(Vector const &v) noexcept;

    protected:
        Vector scale;
    };

    class TransformComp : public Component
    {
    public:
        explicit TransformComp(Vector const &v) noexcept;
        TransformComp(float x, float y) noexcept;

        void setPosition(Vector const &v) noexcept;
        Vector getPosition() const noexcept;

        void Transform(Vector const &v) noexcept;

    protected:
        Vector pos;
    };
} // namespace tnt::ecs2

#endif //!TNT_ECS2_TRANSFORMABLE_HPP