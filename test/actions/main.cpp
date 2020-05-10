#include <thread>
#include <iostream>

#include "utils/Timer.hpp"
#include "utils/Actions.hpp"

using std::chrono::operator""ms;

class Factorial : public tnt::SingleRunAction
{
public:
    explicit Factorial(int num)
        : tnt::SingleRunAction{}, number{num} {}

    inline void Update([[maybe_unused]] long long elapsed_) override
    {
        if (!finished)
        {
            int result{1};
            for (int num{number}; num > 0; --num)
                result = result * num;
            std::cout << number << "! = " << result << '\n';
        }

        finished = true;
    }

private:
    int number;
};

int main()
{
    tnt::Timer timer;
    tnt::ActionList actions;

    tnt::Action *fact7{new Factorial{7}};
    tnt::Action *fact10{new Factorial{10}};

    tnt::BlockingAction *delay{new tnt::Delay{5000}};

    actions.add(fact7);
    actions.add(delay);
    actions.add(fact10);

    bool running{true};
    long long delta{0};

    while (running)
    {
        actions.Update(delta);

        if (actions.empty())
            running = false;

        std::this_thread::sleep_for(100ms);

        delta = timer.deltaTime().count();
        timer.reset();
    }

    delete delay;
    delete fact10;
    delete fact7;

    return 0;
}