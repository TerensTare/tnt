// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "fileIO/Snipper.hpp"

std::recursive_mutex tnt::file::Snipper::mtx;

tnt::file::Snipper::~Snipper() noexcept
{
    std::lock_guard lock{mtx};
    for (auto f : files)
        if (f.second != nullptr)
        {
            delete files[f.first];
            files[f.first] = nullptr;
        }
    files.clear();
    std::map<std::string, File *>{}.swap(files);
}

void tnt::file::Snipper::watchFile(std::string_view name)
{
    std::lock_guard lock{mtx};
    if (files[name.data()])
        return;
    files[name.data()] = new File{name};
}

bool tnt::file::Snipper::isModified(std::string_view file) noexcept
{
    std::lock_guard lock{mtx};
    if (auto pos{files.find(file.data())}; pos == files.end())
        files[file.data()] = new File{file};
    if (auto time{std::filesystem::last_write_time(std::filesystem::path{file})}; time != files[file.data()]->lastTime)
    {
        files[file.data()]->lastTime = time;
        return true;
    }
    return false;
}

void tnt::file::Snipper::unwatchFile(std::string_view filename) noexcept
{
    if (auto it{files.find(filename.data())}; it != files.end())
        it = files.erase(it);
}