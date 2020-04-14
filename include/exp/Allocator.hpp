#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <memory>

namespace tnt
{
    // WARNING: DO NOT USE IT YET. IT IS WIP.
    // TODO:
    // add different utility functions to StackAllocator.
    template <class T, std::size_t S = 64> class StackAllocator
    {
      public:
        StackAllocator() : data{new T[S]}, stack_ptr{0} {}

        ~StackAllocator() noexcept
        {
            delete[] data;
            stack_ptr = 0;
        }

        T *alloc()
        {
            if (stack_ptr < S)
            {
                ++stack_ptr;
                if (!data[stack_ptr])
                    data[stack_ptr] = new T;
                return &data[stack_ptr];
            }
            return nullptr;
        }

        void free()
        {
            if (stack_ptr > 0)
            {
                if (data[stack_ptr])
                {
                    delete data[stack_ptr];
                    data[stack_ptr] = nullptr;
                }
                stack_ptr--;
            }
        }

      private:
        T *data;
        int stack_ptr;
    };
} // namespace tnt

#endif //! ALLOCATOR_HPP