#include <iostream>
#include "exp/Runtimer.hpp"
#include "testlib.h"

int main()
{
    using tnt::rpp::RuntimeManager;

    RuntimeManager *man{};
    man->LoadObject("test.dll");
    bool running{true};

    while (running)
    {
        man->Update();
        std::cout << answer();
    }

    return 0;
}