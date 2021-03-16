#ifndef TNT_PCG_RANDOM_HPP
#define TNT_PCG_RANDOM_HPP

#include <concepts>

#include "core/Config.hpp"
#include "math/Vector.hpp"

// TODO:
// randomColor, etc.
// constexpr halton*

namespace tnt
{
    /// @brief Get a random float on range @c [min_,max_].
    /// @param min_ The minimum value of the float.
    /// @param max_ The maximum value of the float.
    /// @return float
    TNT_API float randomFloat(float min_, float max_);

    /// @brief Get a random int on range @c [min_,max_].
    /// @param min_ The minimum value of the int.
    /// @param max_ The maximum value of the int.
    /// @return int
    TNT_API int randomInt(int min_, int max_);

    /// @brief Get a tnt::Vector with randomly generated coordinates.
    /// @param minX The minimum value of the x coordinate.
    /// @param maxX The maximum value of the x coordinate.
    /// @param minY The minimum value of the y coordinate.
    /// @param maxY The maximum value of the y coordinate.
    /// @return @ref tnt::Vector
    TNT_API Vector randomVector(float minX, float maxX, float minY, float maxY);

    /// @brief Create a randomly generated Vector with magnitude 1.
    /// @return @ref tnt::Vector
    TNT_API Vector randomUnitVector();

    /// @brief Generate a random number using a Halton sequence.
    template <std::integral I>
    inline auto halton1(I const base, I index) noexcept
    {
        I result{0};
        for (I denom{1}; index > 0; index = floor(index / base))
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