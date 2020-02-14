#ifndef CORE_PHYSICS_HPP
#define CORE_PHYSICS_HPP

#if defined(TNT_ACCURATE_PHYSIC)
double typedef real;

#define realpow std::pow
#define realsqrt std::sqrt
#define realsin std::sin
#define realcos std::cos
#define realexp std::exp

#define real0 0.0

#else
float typedef real;

#define realpow std::powf
#define realsqrt std::sqrtf
#define realsin std::sinf
#define realcos std::cosf
#define realexp std::expf

#define real0 0.0f
#endif

#endif //!CORE_PHYSICS_HPP