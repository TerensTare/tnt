// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "fileIO/Snipper.hpp"

namespace fs = std::filesystem;

void tnt::Snipper::watchFile(std::string_view name)
{
    std::lock_guard lock{mtx};
    std::string key{name.data()};
    if (fs::exists(key))
        return;
    files.try_emplace(key, key, fs::last_write_time(key));
}

bool tnt::Snipper::isModified(std::string_view file) noexcept
{
    std::lock_guard lock{mtx};
    std::string key{file.data()};
    if (auto pos{files.find(key)}; pos == files.cend())
        files.try_emplace(key, key, fs::last_write_time(key));

    if (auto time{fs::last_write_time(key)};
        time != files[key].second)
    {
        files[key].second = time;
        return true;
    }
    return false;
}

void tnt::Snipper::unwatchFile(std::string_view filename) noexcept
{
    if (auto it{files.find(filename.data())}; it != files.cend())
        it = files.erase(it);
}