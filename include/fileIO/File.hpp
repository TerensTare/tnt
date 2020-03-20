#ifndef TNT_FILE_HPP
#define TNT_FILE_HPP

#include <filesystem>

// TODO: replace the template parameters with auto and/or concepts and also make Write/Read virtual.
// TODO: add more overloads for Write/Read. (maybe) multiple return types, arrays, etc.

// TODO(maybe):
// also non_movable ??
// bring tnt::file::Writer && tnt::File::Reader HERE.

namespace tnt
{
namespace file
{
class Snipper;
}

class File
{
public:
    File(std::filesystem::path const &path);
    File(std::string_view name);

    File(File const &) = delete;
    File(File &&) = delete;

    operator std::filesystem::path() noexcept;

    template <typename Writer, typename... Args>
    void Write(Writer const &w, Args &&... args) noexcept;

    template <typename Reader, typename T>
    T &Read(Reader const &r, char const *data) const noexcept;

protected:
    std::filesystem::path location;
    std::filesystem::file_time_type lastTime;

    friend class file::Snipper;
};
} // namespace tnt

#endif //!TNT_FILE_HPP