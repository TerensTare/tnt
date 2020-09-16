#ifndef TNT_RUNTIMER_HPP
#define TNT_RUNTIMER_HPP

#include <string_view>
#include <unordered_map>
#include <filesystem>

#include <SDL2/SDL_loadso.h>

// library to handle Hot Code Reloading.
// thx Nicolas Guillemot
// https://github.com/nlguillemot/live_reload_test

// TODO: experimental.
// TODO(test): automatically update RuntimeObject on trigger.
// TODO: keep a file that stores loaded DLL names.
// TODO: free the memory ocupied by *objects.
// TODO: with SDL2, only C libraries are supported. Add support for C++ dll-s.
// TODO: make this asynchronous/thread safe

// TODO(??):
// private RuntimeObject ??
// make this an extension ??
// store loaded object names in the project file ??
// build runtimer as a dll ??
// && RuntimeObject::unload moved to RuntimeManager ??
// Store RuntimeObject-s as a map with their build command as a key ??

namespace tnt::rpp
{
    struct RuntimeObject final
    {
        RuntimeObject(std::string_view path) noexcept;
        ~RuntimeObject() noexcept;

        // TODO: check if Func is a function-like type
        template <typename Func>
        inline Func *Load(std::string_view func) noexcept
        {
            return (Func *)processes.try_emplace(func.data(), SDL_LoadFunction(dll, func.data())).first->second;
        }

        void Update() noexcept;

    private:
        void *dll;
        std::string const binPath;
        std::filesystem::file_time_type lastTime;
        std::unordered_map<std::string, void *> processes;
    };

    // class RuntimeManager
    // {
    // public:
    //   RuntimeManager();
    //   ~RuntimeManager() noexcept;

    //   void LoadObject(char const *name, char const *srcFile);

    //   void *LoadFunction(char const *handle, char const *name);
    //   void Update();
    //   void UpdateObject(char const *name);

    //   void UnloadObject(char const *name);

    // private:
    //   std::unordered_map<std::string, std::string> objectSrc;
    //   std::unordered_map<std::string, RuntimeObject *> objects;
    // };
} // namespace tnt::rpp

#endif //!TNT_RUNTIMER_HPP