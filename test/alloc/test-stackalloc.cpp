#include <iostream>
#include "exp/Allocator.hpp"
#include "math/Vector.hpp"

int main()
{
    tnt::stack_allocator<tnt::Vector> allocator{64};
    tnt::Vector *test[64];
    for (int i{0}; i < 64; ++i)
    {
        test[i] = allocator.allocate();
        std::cout << test[i] << '\n';
        allocator.deallocate();
    }
    return 0;
}