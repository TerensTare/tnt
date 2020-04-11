#ifndef RUNTIMER_HPP
#define RUNTIMER_HPP

#include <string>
#include <filesystem>
#include <unordered_map>

// library to handle Hot Code Reloading.
// author: Terens Tare.

// TODO: experimental.
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

class RuntimeManager
{
public:
    RuntimeManager();
    ~RuntimeManager() noexcept;

    void LoadObject(char const *name, char const *srcFile);

    void *LoadFunction(char const *handle, char const *name);
    void Update();
    void UpdateObject(char const *name);

    void UnloadObject(char const *name);

private:
    struct RuntimeObject
    {
        bool valid;
        std::string bldcmd;
        std::filesystem::file_time_type lastTime;
        void *dll;
    };

    std::unordered_map<std::string, std::string> objectSrc;
    std::unordered_map<std::string, RuntimeObject *> objects;
};
} // namespace tnt::rpp

#endif //!RUNTIMER_HPP