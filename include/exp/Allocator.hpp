#ifndef TNT_ALLOCATOR_HPP
#define TNT_ALLOCATOR_HPP

#include <memory>

namespace tnt
{
// TODO: add different utility functions to stack_allocator.
// ex. deallocate object at given position.
// TODO: make this usable with std:: containers.

// Allocates a raw array pointer with size S on creation and returns the next free member of the allocated array on each allocate() call.
template <class T>
class stack_allocator
{
public:
    static_assert(!std::is_const_v<T>,
                  "The C++ Standard forbids containers of const elements "
                  "because *_allocator<const T> is ill-formed.");

    using value_type = T;

    constexpr stack_allocator(std::size_t S) noexcept
        : stack_ptr{0}
    {
        char *buffer{new char[S * sizeof(T)]};
        data = reinterpret_cast<T **>(&buffer);
    }

    template <typename U>
    constexpr stack_allocator(stack_allocator<U> const &) noexcept = delete;

    template <typename U>
    constexpr stack_allocator &operator=(stack_allocator<U> const &) noexcept = delete;

    constexpr stack_allocator(stack_allocator const &) noexcept = delete;
    constexpr stack_allocator &operator=(stack_allocator const &) noexcept = delete;

    ~stack_allocator() noexcept
    {
        while (--stack_ptr >= 0)
            if (data[stack_ptr] != nullptr)
                data[stack_ptr]->~T();
        delete[] data;
    }

    [[nodiscard]] constexpr T *allocate() noexcept
    {
        data[stack_ptr] = new (data + stack_ptr) T{};
        ++stack_ptr;
        return data[stack_ptr - 1];
    }

    constexpr void deallocate() noexcept
    {
        if (--stack_ptr >= 0)
        {
            data[stack_ptr]->~T();
            data[stack_ptr] = nullptr;
        }
    }

private:
    T **data;
    int stack_ptr;
};
} // namespace tnt

#endif //!TNT_ALLOCATOR_HPP