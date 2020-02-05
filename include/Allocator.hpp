#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <memory>

namespace tnt
{

template <class T>
class StackAllocator
{
public:
    void init()
    {
        data = std::malloc(sizeof(T) * 64);
        stack_ptr = 0;
    }

    void close()
    {
        std::free(data);
        stack_ptr = 0;
    }

    T *alloc()
    {
        if (stack_ptr < 64)
        {
            stack_ptr--;
            return &data[stack_ptr - 1];
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