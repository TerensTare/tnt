// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "core/Space.hpp"
#include "ecs/Object.hpp"
#include "core/Camera.hpp"

tnt::Space::~Space() noexcept
{
    std::map<std::string, Object *>{}.swap(objects);
}

void tnt::Space::addObject(std::pair<std::string_view, tnt::Object *> const &obj) { objects.insert(obj); }

void tnt::Space::addObject(std::string_view id, tnt::Object *obj)
{
    objects.insert(std::make_pair(id, obj));
}

tnt::Object *tnt::Space::getObject(std::string_view id) const { return objects.at(id.data()); }

void tnt::Space::removeObject(std::string_view id) noexcept
{
    if (auto it{objects.find(id.data())}; it != objects.end())
        it = objects.erase(it);
}

void tnt::Space::Draw(tnt::camera auto const *cam)
{
    for (auto const &it : objects)
        if (it.second->isActive())
            ;
}

void tnt::Space::Update(long long time_)
{
    for (auto const &it : objects)
        if (it.second->isActive())
            it.second->Update(time_);
}