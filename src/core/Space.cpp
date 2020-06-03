// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <SDL2/SDL.h>

#include "core/Space.hpp"
#include "core/Camera.hpp"

#include "ecs/RigidBody.hpp"

bool tnt::Space::isActive() const noexcept { return active; }

void tnt::Space::addObject(std::pair<std::string_view, tnt::Object *> const &obj) { objects.insert(obj); }

void tnt::Space::addObject(std::string_view id, tnt::Object *obj)
{
    objects.insert(std::make_pair(id, obj));
}

tnt::Object *tnt::Space::getObject(std::string_view id) const { return objects.find(id.data())->second; }

void tnt::Space::removeObject(std::string_view id) noexcept
{
    if (auto it{objects.find(id.data())}; it != objects.end())
        it = objects.erase(it);
}

void tnt::Space::Update(long long time_) noexcept
{
    for (auto const &it : objects)
        if (it.second->isActive())
        {
            active = true;
            if (it.second->has<PhysicsComponent>())
            {
                PhysicsComponent *phys{it.second->get<PhysicsComponent>()};
                phys->doPhysics(time_);
                it.second->Transform(phys->getVelocity() * time_);
            }
            it.second->Update(time_);
        }
        else
            active = false;
}