#include <iostream>

#include "exp/Runtimer.hpp"
#include "utils/Logger.hpp"

#undef main

int main()
{
    using tnt::rpp::RuntimeManager;

    RuntimeManager *man{};
    man->LoadObject("test");
    int (*answer)() = (int (*)())man->LoadFunction("test", "answer");

    bool running{true};

    while (running)
    {
        man->UpdateObject("test");

        if (answer)
            std::cout << answer();
        else
            tnt::logger::error("int answer() couldn't be loaded correctly! Error: {}.\n exiting...", GetLastError());

        if (answer() == 43)
        {
            tnt::logger::info("Succesfully exiting...");
            running = false;
        }
        else
            continue;
    }

    return 0;
}