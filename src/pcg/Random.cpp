// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <random>
#include "pcg/Random.hpp"

float tnt::randomFloat(float min_, float max_)
{
    static std::random_device device;
    static std::default_random_engine eng{device()};
    static std::uniform_real_distribution<float> dist{min_, std::nextafter(max_, std::numeric_limits<float>::max())};
    return dist(eng);
}

int tnt::randomInt(int min_, int max_)
{
    static std::random_device device;
    static std::default_random_engine generator{device()};
    static std::uniform_int_distribution dist{min_, max_};
    return dist(generator);
}

tnt::Vector tnt::randomVector(float minX, float maxX, float minY, float maxY)
{
    return {randomFloat(minX, maxX), randomFloat(minY, maxY)};
}

tnt::Vector tnt::randomUnitVector()
{
    float const &angle{randomFloat(0.f, 2_pi)};
    return {cosf(angle), sinf(angle)};
}