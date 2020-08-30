// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "exp/Runtimer.hpp"

#include <iostream>

namespace fs = std::filesystem;

tnt::rpp::RuntimeObject::RuntimeObject(std::string_view path) noexcept
    : binPath{path.data()}
{
    if (dll = SDL_LoadObject(path.data()); !dll)
        std::cout << "Error loading " << path << "!! " << SDL_GetError();
}

tnt::rpp::RuntimeObject::~RuntimeObject() noexcept
{
    SDL_UnloadObject(dll);
}

void tnt::rpp::RuntimeObject::Update() noexcept
{
    if (fs::path const &dll_path{binPath};
        fs::exists(dll_path))
    {
        if (fs::file_time_type const &last{fs::last_write_time(dll_path)};
            last != lastTime)
        {
            if (std::string const &tmp{binPath + "_tmp.dll"};
                fs::copy_file(binPath, tmp))
            {
                if (void *temp{SDL_LoadObject(tmp.c_str())};
                    temp)
                {
                    for (auto const &[proc, _] : processes)
                        processes[proc] = SDL_LoadFunction(temp, proc.c_str());

                    SDL_UnloadObject(dll);
                    dll = temp;
                    lastTime = last;
                }
            }
        }
    }
}

// tnt::rpp::RuntimeManager::RuntimeManager()
// {
//     for (auto it : objects)
//         it.second = nullptr;
// }

// tnt::rpp::RuntimeManager::~RuntimeManager() noexcept
// {
//     for (auto obj : objects)
//         if (obj.second != nullptr)
//             UnloadObject(obj.first.c_str());
//     objects.clear();

//     for (auto src : objectSrc)
//         if (!src.second.empty())
//             src.second.clear();
//     objectSrc.clear();

//     std::cout << std::flush;
// }

// void tnt::rpp::RuntimeManager::LoadObject(char const *name, char const *srcFile)
// {
//     if (objects[name] == nullptr || objects[name]->valid == false)
//     {
//         std::cout << "loading " << name << ".dll\n";

//         std::filesystem::copy(
//             std::filesystem::path{std::string{name}.append(".dll")},
//             std::filesystem::path{std::string{name}.append("_temp.dll")});

//         objects[name]->dll = SDL_LoadObject(std::string{name}.append(".dll").c_str());
//         objects[name]->bldcmd = std::string{"cl /EHsc /std:c++17 /DDLL_EXPORT "}.append(srcFile).append(" /link /DLL"); // fmt::format("cl /EHsc /std:c++17 {} /DDLL_EXPORT /link /DLL", srcFile).c_str();
//         objects[name]->lastTime = std::filesystem::last_write_time(srcFile);
//         objects[name]->valid = true;
//     }
// }

// void *tnt::rpp::RuntimeManager::LoadFunction(char const *handle, char const *name)
// {
//     void *result;
//     if (objects[handle] == nullptr || objects[handle]->valid == false)
//     {
//         std::cout << "LoadFunction(" << name << ") at " << handle
//                   << " is nullptr!! Error: " << SDL_GetError() << '\n';
//         result = nullptr;
//     }

//     result = SDL_LoadFunction(objects[handle]->dll, name);
//     return result;
// }

// void tnt::rpp::RuntimeManager::Update()
// {
//     for (auto it : objects)
//         if (std::filesystem::last_write_time(objectSrc.find(it.first)->second) != it.second->lastTime)
//             UpdateObject(it.first.c_str());
// }

// void tnt::rpp::RuntimeManager::UpdateObject(char const *name)
// {
//     if (objects[name] != nullptr)
//     {
//         UnloadObject(name);
//         objects[name] = nullptr;
//     }
//     std::cout << "running " << objects[name]->bldcmd;
//     system(objects[name]->bldcmd.c_str());
//     LoadObject(name, objectSrc[name].c_str());
// }

// void tnt::rpp::RuntimeManager::UnloadObject(char const *name)
// {
//     std::cout << "unloading " << name << ".dll\n";
//     SDL_UnloadObject(objects[name]->dll);
//     objects[name]->valid = false;
//     // delete objects[name];
// }