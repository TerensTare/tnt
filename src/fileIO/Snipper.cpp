// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <mutex>
#include <map>

#include "fileIO/Snipper.hpp"

namespace fs = std::filesystem;
using file = std::pair<std::string, std::filesystem::file_time_type>;

struct tnt::Snipper::impl
{
    std::recursive_mutex mtx;
    std::map<std::string, file, std::less<>> files;
};

void tnt::Snipper::watchFile(std::string_view name)
{
    std::lock_guard lock{data->mtx};
    std::string key{name.data()};
    if (fs::exists(key))
        data->files.try_emplace(key, key, fs::last_write_time(key));
}

bool tnt::Snipper::isModified(std::string_view file) noexcept
{
    std::lock_guard lock{data->mtx};
    std::string const key{file.data()};
    if (data->files.find(key) == data->files.cend())
    {
        data->files.try_emplace(key, key, fs::last_write_time(key));
        return false; // since we just added the file, there's no chance it has been modified
    }

    if (auto time{fs::last_write_time(key)};
        time != data->files[key].second)
    {
        data->files[key].second = time;
        return true;
    }
    return false;
}

void tnt::Snipper::unwatchFile(std::string_view filename) noexcept
{
    std::lock_guard lock{data->mtx};
    if (auto it{data->files.find(filename.data())}; it != data->files.cend())
        it = data->files.erase(it);
}