#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <memory>

namespace tnt
{
// WARNING: DO NOT USE IT YET. IT IS WIP.
template <class T, std::size_t S = 64>
class StackAllocator
{
public:
    StackAllocator()
        : data{std::malloc(sizeof(T) * S)}, stack_ptr{0} {}

    ~StackAllocator() noexcept(std::free(data))
    {
        std::free(data);
        stack_ptr = 0;
    }

    T *alloc()
    {
        if (stack_ptr < S)
        {
            ++stack_ptr;
            return &data[stack_ptr];
        }
        return nullptr;
    }

    void free()
    {
        if (stack_ptr > 0)
            stack_ptr--;
    }

private:
    T *data;
    int stack_ptr;
};
} // namespace tnt

#endif //!ALLOCATOR_HPP