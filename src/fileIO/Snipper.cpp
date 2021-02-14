// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "fileIO/Snipper.hpp"

namespace fs = std::filesystem;

void tnt::Snipper::watchFile(std::string_view name)
{
    mtx.lock();
    hashed_string key{name.data()};
    if (fs::exists(key.data()))
        files.try_emplace(key.hash(), key.data(), fs::last_write_time(key.data()));
    mtx.unlock();
}

bool tnt::Snipper::isModified(std::string_view file) noexcept
{
    std::lock_guard lock{mtx};
    hashed_string key{file.data()};
    if (files.find(key.hash()) == files.cend())
    {
        files.try_emplace(key.hash(), key.data(), fs::last_write_time(key.data()));
        return false; // since we just added the file, there's no chance it has been modified
    }

    if (auto time{fs::last_write_time(key.data())};
        time != files[key.hash()].second)
    {
        files[key.hash()].second = time;
        return true;
    }
    return false;
}

void tnt::Snipper::unwatchFile(std::string_view filename) noexcept
{
    std::lock_guard lock{mtx};
    if (auto it{files.find(fnv1a<char>(filename.data()))}; it != files.cend())
        it = files.erase(it);
}