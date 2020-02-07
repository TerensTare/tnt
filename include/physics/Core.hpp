#ifndef CORE_PHYSICS_HPP
#define CORE_PHYSICS_HPP

#if defined(TNT_ACCURATE_PHYSIC)
double typedef real;
#define pow std::pow
#else
float typedef real;
#define pow std::powf
#endif

#endif //!CORE_PHYSICS_HPP