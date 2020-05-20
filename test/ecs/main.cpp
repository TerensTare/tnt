#include <sol/sol.hpp>
#include "ecs/Object.hpp"

class Test : public tnt::Component
{
public:
    explicit Test(int value_) : value{value_} {}
    ~Test() noexcept { tnt::logger::debug("delete-ing Component Test"); }

    void setValue(int value_) noexcept { value = value_; }
    int getValue() const noexcept { return value; }

private:
    int value;
};

class Player : public tnt::Object
{
public:
    explicit Player(int value, std::string_view filename) noexcept
        : test{add<Test>(value)}, script{add<tnt::Script>(filename)} {}

    ~Player() noexcept
    {
        clear();
        if (script != nullptr)
            tnt::logger::info("Script just leaked!!");
    }

    void setTest(Test *test_) noexcept { test = test_; }
    Test *getTest() const noexcept { return test; }

    virtual void Update(long long time_) noexcept
    {
        int val{test->getValue()};
        test->setValue(++val);
        tnt::logger::info("Value: {}", test->getValue());
        script->Update(time_);
    }

private:
    Test *test;
    tnt::Script *script;
};

int main()
{
    Player player{10, "player.lua"};

    for (int num{0}; num < 1000; ++num)
        player.Update(num);

    return 0;
}