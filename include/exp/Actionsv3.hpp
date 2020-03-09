#ifndef TNT_ACTIONS_V3_HPP
#define TNT_ACTIONS_V3_HPP

#include <queue>
#include <memory>

// TODO: conditional split, delay, sync, resumable
// TODO: operator |, operator ==
// TODO: lanes
// TODO: if this implementation is succesful, move all function definitions to the cpp file.
// TODO: utility functions: isFirst(), etc

// https://app.gitmind.com/doc/452888c3892ee36a7b91ae828857985f

namespace tnt
{
class ActionList final;

class __declspec(novtable) Action
{
public:
    Action() : finished{false} {}

    virtual ~Action() noexcept {}

    virtual void Update(float dt) = 0;

    bool Finished() const noexcept { return finished; }

protected:
    bool blocking;
    bool finished;
    std::weak_ptr<ActionList> owner;
};

class ActionList final
{
public:
    void Add(Action *action) { members.push(std::make_shared<Action>(action)); }
    void Update(float dt)
    {
        if (!members.empty())
        {
            members.front()->Update(dt);
            if (members.front()->Finished())
                members.pop();
        }
    }

    decltype(auto) front() noexcept(noexcept(members.front())) { return members.front(); }

private:
    std::queue<std::shared_ptr<Action>> members;
};

namespace actions
{
class __declspec(novtable) NonBlocking : public Action
{
public:
    NonBlocking() : Action() { blocking = false; }
};

class __declspec(novtable) Blocking : public Action
{
public:
    Blocking() : Action() { blocking = true; }
};

// maybe without __declspec(novtable) ??
class __declspec(novtable) Sync : public Action
{
public:
    // final maybe ??
    virtual void Update()
    {
        if (auto li{owner.lock()}; li && &*li->front() == this)
            finished = true;
    }
};
} // namespace actions
} // namespace tnt

#endif //!TNT_ACTIONS_V3_HPP