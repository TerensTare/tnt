#ifndef RUNTIMER_HPP
#define RUNTIMER_HPP

#include <filesystem>
#include <unordered_map>

// library to handle Hot Code Reloading.
// author: Terens Tare.

// TODO: experimental.
// TODO: make it cross-platform by using SDL2.
// TODO: automatically update RuntimeObject on trigger.
// TODO: keep a file that stores loaded DLL names.
// TODO: handle dll-s without header files, that's how it should be done.
// TODO: Copy the original dll to a temporary one before reloading.
// TODO: rebuild the dll on the update function.

// TODO(??):
// typename RuntimeObject::dll should be std::any ??
// private RuntimeObject ??
// make this an extension ??
// store loaded object names in the project file ??
// C Structure RuntimeObject
// && RuntimeObject::unload moved to RuntimeManager ??

namespace tnt::rpp
{
using std::filesystem::file_time_type;

class RuntimeObject
{
public:
    RuntimeObject(char const *dllName);
    ~RuntimeObject() noexcept;

    RuntimeObject(RuntimeObject const &) = delete;
    RuntimeObject &operator=(RuntimeObject const &) = delete;

    RuntimeObject(RuntimeObject &&) = delete;
    RuntimeObject &operator=(RuntimeObject &&) = delete;

private:
    void unload();

    void *dll;
    bool valid;
    file_time_type lastTime;

    friend class RuntimeManager;
};

class RuntimeManager
{
public:
    void LoadObject(char const *name);

    void Update();
    void UpdateObject(char const *name);

private:
    std::unordered_map<char const *, RuntimeObject *> objects;
};
} // namespace tnt::rpp

#endif //!RUNTIMER_HPP