#ifndef TNT_PCG_RANDOM_HPP
#define TNT_PCG_RANDOM_HPP

#include <random>

// TODO: randomVector, randomColor, etc.

namespace tnt
{

float randomFloat(float min_, float max_) noexcept
{
    static std::default_random_engine eng;
    static std::uniform_real_distribution<float> dist{min_, std::nextafter(max_, std::numeric_limits<float>::max())};
    return dist(eng);
}
int randomValue(int min, int max) noexcept
{
    std::random_device device;
    std::mt19937 generator{device()};
    std::uniform_int_distribution dist{min, max};
    return dist(generator);
}
} // namespace tnt

#endif //!TNT_PCG_RANDOM_HPP