// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "exp/Runtimer.hpp"
#include "utils/Logger.hpp"

tnt::rpp::RuntimeManager::~RuntimeManager() noexcept
{
    for (auto obj : objects)
        if (obj.second != nullptr)
            UnloadObject(obj.first);
    objects.clear();
}

void tnt::rpp::RuntimeManager::LoadObject(char const *name)
{
    if (objects[name] == nullptr || objects[name]->valid == false)
    {
        tnt::logger::info("loading {}.dll", name);
        objects[name]->dll = LoadLibrary(fmt::format("{}.dll", name).c_str());
        objects[name]->bldcmd = fmt::format("cl /EHsc /std:c++17 {}.cpp /link /DLL", name).c_str();
        objects[name]->lastTime = std::filesystem::last_write_time(fmt::format("{}.cpp", name));
        objects[name]->valid = true;
    }
}

FARPROC tnt::rpp::RuntimeManager::LoadFunction(const char *handle, const char *name)
{
    if (objects[handle] == nullptr || objects[handle]->valid == false)
    {
        tnt::logger::info("LoadFunction({}) is returning nullptr", name);
        return nullptr;
    }
    return GetProcAddress(objects[handle]->dll, name);
}

void tnt::rpp::RuntimeManager::Update()
{
    for (auto it : objects)
        if (std::filesystem::last_write_time(fmt::format("{}.cpp", it.first)) != it.second->lastTime)
            UpdateObject(it.first);
}

void tnt::rpp::RuntimeManager::UpdateObject(char const *name)
{
    if (objects[name] != nullptr)
    {
        UnloadObject(name);
        objects[name] = nullptr;
    }
    tnt::logger::info("running {}", objects[name]->bldcmd);
    system(objects[name]->bldcmd);
    LoadObject(name);
}

void tnt::rpp::RuntimeManager::UnloadObject(char const *name)
{
    tnt::logger::info("unloading {}.dll", name);
    FreeLibrary(objects[name]->dll);
    objects[name]->valid = false;
}