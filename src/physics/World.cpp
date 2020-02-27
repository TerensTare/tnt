// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "physics/World.hpp"

void tnt::World::startFrame()
{
    auto reg{first};
    while (reg != nullptr)
    {
        reg->particle->clearAccumulator();
        reg = reg->next;
    }
}