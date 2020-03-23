#ifndef TNT_PCG_RANDOM_HPP
#define TNT_PCG_RANDOM_HPP

#include <random>

// TODO: randomVector, randomColor, etc.

namespace tnt
{

int randomValue(int min, int max) noexcept
{
    std::random_device device;
    std::mt19937 generator{device()};
    std::uniform_int_distribution dist{min, max};
    return dist(generator);
}
} // namespace tnt

#endif //!TNT_PCG_RANDOM_HPP