#include "exp/Allocator.hpp"
#include "math/Vector.hpp"

int main()
{
    tnt::StackAllocator<tnt::Vector> allocator;
    auto vec{*allocator.alloc()};
    vec.x = 7;
    vec.y = 8;

    allocator.free();
    return 0;
}