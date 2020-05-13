// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs/Object.hpp"

tnt::Object::~Object() noexcept
{
    if (active)
        active = false;
    if (parent != nullptr)
    {
        delete parent;
        parent = nullptr;
    }
    clear();
}

tnt::Vector tnt::Object::getPosition(tnt::Object::Coordinates const &coords) const noexcept
{
    if (parent == nullptr || coords == Coordinates::LOCAL)
        return position;

    Vector parentScale{parent->getScale(Coordinates::GLOBAL)};
    Vector rotateVector{position.x * parentScale.x, position.y * parentScale.y};
    Vector rotatePos{RotateVector(rotateVector, parent->getAngle(Coordinates::LOCAL))};

    return rotatePos + parent->getPosition(Coordinates::GLOBAL);
}

void tnt::Object::setPosition(Vector const &pos) noexcept { position = pos; }

void tnt::Object::Transform(Vector const &pos) noexcept { position = position + pos; }

void tnt::Object::setAngle(float radian) noexcept
{
    angle = radian;
    while (fabsf(angle - 360.f) > FLT_EPSILON)
        angle = angle - 360.f;
}

float tnt::Object::getAngle(tnt::Object::Coordinates const &coords) const noexcept
{
    if (parent == nullptr || coords == Coordinates::LOCAL)
        return angle;

    return angle + parent->getAngle(Coordinates::GLOBAL);
}

void tnt::Object::Rotate(float radian) noexcept
{
    angle = angle + radian;
    while (fabsf(angle - 360.f) > FLT_EPSILON)
        angle = angle - 360.f;
}

void tnt::Object::setScale(Vector const &ratio) noexcept { scale = ratio; }

tnt::Vector tnt::Object::getScale(tnt::Object::Coordinates const &coords) const noexcept
{
    if (parent == nullptr || coords == Coordinates::LOCAL)
        return scale;

    Vector parentScale{parent->getScale(Coordinates::GLOBAL)};

    return tnt::Vector{scale.x * parentScale.x, scale.y * parentScale.y};
}

void tnt::Object::Scale(tnt::Vector const &ratio) noexcept { scale = scale + ratio; }

tnt::Object *tnt::Object::getParent() const noexcept { return parent; }

void tnt::Object::setParent(Object *parent_) noexcept
{
    if (parent_ == nullptr)
    {
        position = getPosition(Coordinates::GLOBAL);
        scale = getScale(Coordinates::GLOBAL);
        angle = getAngle(Coordinates::GLOBAL);
    }
    else
    {
        if (parent != nullptr)
            setParent(nullptr);
        Vector parentScale{parent_->getScale(Coordinates::GLOBAL)};
        Vector rotateVector{getPosition(Coordinates::GLOBAL) - parent_->getPosition(Coordinates::GLOBAL)};

        position = RotateVector(rotateVector, -parent_->getAngle(Coordinates::GLOBAL));
        position.x = position.x / parentScale.x;
        position.y = position.y / parentScale.y;

        angle = angle - parent_->getAngle(Coordinates::GLOBAL);
        scale = Vector{scale.x / parentScale.x, scale.y / parentScale.y};
    }

    parent = parent_;
}

bool tnt::Object::isActive() const noexcept { return active; }
void tnt::Object::setActive(bool active_) noexcept { active = active_; }