#ifndef TNT_FILE_SNIPPER_HPP
#define TNT_FILE_SNIPPER_HPP

#include <map>
#include <mutex>
#include "fileIO/File.hpp"

// TODO:
// make this class's operations asynchronous.
// support watching directories.
// void onModify() specialization.
// arguments for lambda at onModify.
// onModify() noexcept ONLY IF the callback is noexcept.

// TODO(maybe):
// a singleton. ??
// final class ??

namespace tnt::file
{
class Snipper
{
public:
    ~Snipper() noexcept;

    void watchFile(std::string_view name);

    template <typename Func>
    auto onModify(std::string_view file, Func func) noexcept(noexcept(func())) -> decltype(func())
    {
        std::lock_guard lock{mtx};
        if (isModified(file))
            func();
    }

private:
    bool isModified(std::string_view file) noexcept;

    std::map<std::string, File *> files;
    static std::recursive_mutex mtx;
};
} // namespace tnt::file

#endif //!TNT_FILE_SNIPPER_HPP