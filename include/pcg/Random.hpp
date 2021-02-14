#ifndef TNT_PCG_RANDOM_HPP
#define TNT_PCG_RANDOM_HPP

#include <random>
#include "math/Vector.hpp"

// TODO: randomColor, etc.
// TODO: split this into header/source.

namespace tnt
{
    /// @brief Get a random float on range @c [min_,max_].
    /// @param min_ The minimum value of the float.
    /// @param max_ The maximum value of the float.
    /// @return float
    inline float randomFloat(float min_, float max_)
    {
        static std::random_device device;
        static std::default_random_engine eng{device()};
        static std::uniform_real_distribution<float> dist{min_, std::nextafter(max_, std::numeric_limits<float>::max())};
        return dist(eng);
    }

    /// @brief Get a random int on range @c [min_,max_].
    /// @param min_ The minimum value of the int.
    /// @param max_ The maximum value of the int.
    /// @return int
    inline int randomInt(int min_, int max_)
    {
        static std::random_device device;
        static std::default_random_engine generator{device()};
        static std::uniform_int_distribution dist{min_, max_};
        return dist(generator);
    }

    /// @brief Get a tnt::Vector with randomly generated coordinates.
    /// @param minX The minimum value of the x coordinate.
    /// @param maxX The maximum value of the x coordinate.
    /// @param minY The minimum value of the y coordinate.
    /// @param maxY The maximum value of the y coordinate.
    /// @return @ref tnt::Vector
    inline tnt::Vector randomVector(float minX, float maxX, float minY, float maxY)
    {
        return {randomFloat(minX, maxX), randomFloat(minY, maxY)};
    }

    /// @brief Create a randomly generated Vector with magnitude 1.
    /// @return @ref tnt::Vector
    inline tnt::Vector randomUnitVector()
    {
        float const &angle{randomFloat(0.f, 2_pi)};
        return Vector{cosf(angle), sinf(angle)};
    }

    /// @brief Generate a random number using a Halton sequence.
    template <std::integral I>
    inline auto halton1(I const base, I index) noexcept
    {
        decltype(base) result{0};
        for (decltype(base) denom{1};
             index > 0; index = floor(index / base))
        {
            denom *= base;
            result += (index % base) / denom;
        }
        return result;
    }

    /// @brief Generate a random Vector using Halton sequences.
    template <std::integral I>
    inline Vector halton2(I const baseX, I const baseY, I index) noexcept
    {
        return {(float)halton1<I>(baseX, index), (float)halton1<I>(baseY, index)};
    }
} // namespace tnt

#endif //!TNT_PCG_RANDOM_HPP