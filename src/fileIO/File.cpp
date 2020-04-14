// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "fileIO/File.hpp"

namespace fs = std::filesystem;

tnt::File::File(fs::path const& path)
    : location { path }
    , lastTime { fs::last_write_time(path) }
{
}

tnt::File::File(std::string_view name)
    : location { name }
    , lastTime { fs::last_write_time(location) }
{
}

tnt::File::operator std::filesystem::path() noexcept
{
    return location;
}

template <class Writer, class... Args>
void tnt::File::Write(Writer const& w, Args&&... args) noexcept
{
    w.write(std::forward<Args...>(args...));
}

template <class Reader, class T>
T& tnt::File::Read(const Reader& r, const char* data) const noexcept
{
    return r.read(data);
}