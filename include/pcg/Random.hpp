#ifndef TNT_PCG_RANDOM_HPP
#define TNT_PCG_RANDOM_HPP

#include <random>

// TODO: randomVector, randomColor, etc.

namespace tnt
{

    /// @brief Get a random float on range @c [min_,max_].
    /// @param min_ The minimum value of the float.
    /// @param max_ The maximum value of the float.
    /// @return float
    inline float randomFloat(float min_, float max_) noexcept
    {
        static std::default_random_engine eng;
        static std::uniform_real_distribution<float> dist{min_, std::nextafter(max_, std::numeric_limits<float>::max())};
        return dist(eng);
    }

    /// @brief Get a random int on range @c [min_,max_].
    /// @param min_ The minimum value of the int.
    /// @param max_ The maximum value of the int.
    /// @return int
    inline int randomValue(int min, int max) noexcept
    {
        static std::random_device device;
        static std::mt19937 generator{device()};
        static std::uniform_int_distribution dist{min, max};
        return dist(generator);
    }
} // namespace tnt

#endif //!TNT_PCG_RANDOM_HPP