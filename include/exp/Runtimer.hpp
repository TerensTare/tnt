#ifndef TNT_RUNTIMER_HPP
#define TNT_RUNTIMER_HPP

#include <string_view>
#include <unordered_map>
#include <filesystem>
#include <vector>

// library to handle Hot Code Reloading.
// thx Nicolas Guillemot
// https://github.com/nlguillemot/live_reload_test

// TODO: experimental.
// TODO: handle Linux and MacOSX cases.
// TODO(test): automatically update RuntimeObject on trigger.
// TODO: keep a file that stores loaded DLL names.
// TODO(test): handle dll-s without header files, that's how it should be done.
// TODO: Copy the original dll to a temporary one before reloading.
// TODO(test): rebuild the dll on the update function.
// TODO: avoid using new/delete.
// TODO: free the memory ocupied by *objects.
// TODO: with SDL2, only C libraries are supported. Add support for C++ dll-s.

// TODO(??):
// typename RuntimeObject::dll should be std::any ??
// private RuntimeObject ??
// make this an extension ??
// store loaded object names in the project file ??
// build runtimer as a dll ??
// C Structure RuntimeObject
// && RuntimeObject::unload moved to RuntimeManager ??
// Store RuntimeObject-s as a map with their build command as a key ??

namespace tnt::rpp
{
struct RuntimeObject
{
    bool valid;
    std::string binPath;
    std::filesystem::file_time_type lastTime;
    void *dll;
    std::vector<std::string> procsToLoad;
    std::unordered_map<std::string, void *> processes;
};

void Update(RuntimeObject *obj) noexcept;

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