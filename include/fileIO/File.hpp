#ifndef FILE_HPP
#define FILE_HPP

namespace tnt
{
class File
{
public:
    virtual ~File() noexcept;

    template <typename... Args>
    void Write(Args &&... args) noexcept;

    template <typename T>
    T Read(char const *data) const noexcept;
};
} // namespace tnt

#endif //!FILE_HPP