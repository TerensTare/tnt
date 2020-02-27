// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <SDL2/SDL_loadso.h>
#include "exp/Runtimer.hpp"

tnt::rpp::RuntimeObject::RuntimeObject(char const *dllName)
    : dll{SDL_LoadObject(dllName)},
      lastTime{std::filesystem::last_write_time(dllName)},
      valid{true}
{
}

tnt::rpp::RuntimeObject::~RuntimeObject() noexcept
{
    unload();
}

void tnt::rpp::RuntimeObject::unload()
{
    SDL_UnloadObject(dll);
    valid = false;
}

void tnt::rpp::RuntimeManager::LoadObject(char const *name)
{
    if (objects[name] == nullptr)
        objects[name] = new RuntimeObject(name);
}

void tnt::rpp::RuntimeManager::Update()
{
    for (auto it : objects)
        if (std::filesystem::last_write_time(it.first) != it.second->lastTime)
            UpdateObject(it.first);
}

void tnt::rpp::RuntimeManager::UpdateObject(char const *name)
{
    if (objects[name] != nullptr)
    {
        objects[name]->unload();
        delete objects[name];
        objects[name] = nullptr;
    }
    objects[name] = new RuntimeObject{name};
}