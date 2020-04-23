#include <iostream>
#include <SDL2/SDL.h>
#include "testlib.h"
#include "exp/Runtimer.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    using namespace tnt::rpp;

    RuntimeObject *object;
    object->binPath = "test.dll";
    object->procsToLoad.push_back("answer");

    while (true)
    {
        tnt::rpp::Update(object);
        auto live_code{(decltype(answer) *)(object->processes["answer"])};
        if (live_code)
            live_code();
        SDL_Delay(1000);
    }

    return 0;
}