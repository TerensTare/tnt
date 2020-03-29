// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs/Object.hpp"

tnt::Object::~Object() noexcept
{
    parent.reset(); // should this be like this? We don't want do delete the parent, just the pointer.
}

void tnt::Object::setParent(std::shared_ptr<Object> obj) noexcept { parent = obj; }
tnt::Object *tnt::Object::getParent() const noexcept { return parent.lock().get(); }