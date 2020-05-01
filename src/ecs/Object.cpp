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
}

tnt::Vector tnt::Object::getPosition() const noexcept { return position; }
void tnt::Object::setPosition(Vector const &pos) noexcept { position = pos; }

void tnt::Object::Transform(Vector const &pos) noexcept { position = position + pos; }

tnt::Object *tnt::Object::getParent() const noexcept { return parent; }
void tnt::Object::setParent(Object *parent_) noexcept { parent = parent_; }

bool tnt::Object::isActive() const noexcept { return active; }
void tnt::Object::setActive(bool active_) noexcept { active = active_; }