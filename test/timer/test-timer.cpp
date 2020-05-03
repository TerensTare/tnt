#include <iostream>
#include <thread>

#include "utils/Timer.hpp"

int main()
{
    using tnt::Timer;
    using namespace std::literals;

    for (auto it{100ms}; it <= 1s; it += 100ms)
    {
        Timer timer;
        std::this_thread::sleep_for(it);
        auto time{timer.deltaTime<std::chrono::milliseconds>()};
        std::cout << "elapsed time: " << (time - it).count() << "\n";
    }
    return 0;
}