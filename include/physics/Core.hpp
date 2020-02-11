#ifndef CORE_PHYSICS_HPP
#define CORE_PHYSICS_HPP

#if defined(TNT_ACCURATE_PHYSIC)
double typedef real;

#define pow std::pow

#define real0 0.0
#else
float typedef real;

#define pow std::powf
#define real0 0.0f
#endif

#endif //!CORE_PHYSICS_HPP