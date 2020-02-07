// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "physics/Firework.hpp"

bool tnt::Firework::update(real duration)
{
    integrate(duration);
    age -= duration;
    return (age < 0);
}