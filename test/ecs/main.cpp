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
    explicit Player(int value) : test{add<Test>(value)} {}
    ~Player() noexcept { clear(); }

    void setTest(Test *test_) noexcept { test = test_; }
    Test *getTest() const noexcept { return test; }

    virtual void Update([[maybe_unused]] long long time_) noexcept
    {
        int val{test->getValue()};
        test->setValue(++val);
        tnt::logger::info("Value: {}", test->getValue());
    }

private:
    Test *test;
};

int main()
{
    Player *player{new Player{10}};

    for (int num{0}; num < 1000; ++num)
        player->Update(10);

    delete player;
    return 0;
}