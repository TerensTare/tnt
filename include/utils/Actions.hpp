#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include <vector>

// TODO:
// Maybe separate ResumableAction from Action.
// Make unblocking and blocking Actions using C++20 coroutines.

// IDEA:
// Trigger a new action when an action is finished(that's why End() is for).

namespace tnt
{
class ActionList;

class Action
{
public:
    Action(bool blocking);
    virtual ~Action() {}

    virtual void Update(float delta_t) = 0;
    virtual void Start();
    virtual void End();

    void Finish();

    void Block();
    void Unblock();

    void Pause();
    void Resume();
    void Cancel();

    bool Blocks() const noexcept { return blocks; }
    bool Blocked() const noexcept { return blocked; }
    bool Complete() const noexcept;
    bool Started() const noexcept;
    bool Paused() const noexcept;
    bool Finished() const noexcept { return finished; }

    int Lanes() const noexcept { return lanes; }
    int BlockedBy() const noexcept { return blockedmask; }
    int Blocking() const noexcept { return blockmask; }

protected:
    bool finished;
    bool blocks, blocked;
    int lanes;
    int blockmask, blockedmask; // TODO: init these.
    ActionList owner;
};

class Sync : public Action
{
public:
    void Update(float delta_t) override;
};

class Delay : public Action
{
public:
    Delay(float time);
    void Update(float delta_t) override;

private:
    float elapsed;
    float duration;
};

class ResumableAction : public Action
{
public:
    virtual void Suspend() = 0;
    virtual void Resume() = 0;

    bool Paused() const noexcept { return suspended; }
    bool Unpaused() const noexcept { return !suspended; }

protected:
    bool suspended;
};

class HierarchicalAction : public Action
{
public:
    virtual void Update(float delta_t) override;
};

class ActionList : public Action
{
public:
    ~ActionList() noexcept;

    decltype(auto) begin() const noexcept { return actions.begin(); }
    decltype(auto) end() const noexcept { return actions.end(); }
    decltype(auto) rbegin() const noexcept { return actions.rbegin(); }
    decltype(auto) rend() const noexcept { return actions.rend(); }

    decltype(auto) cbegin() const noexcept { return actions.cbegin(); }
    decltype(auto) cend() const noexcept { return actions.cend(); }
    decltype(auto) crbegin() const noexcept { return actions.crbegin(); }
    decltype(auto) crend() const noexcept { return actions.crend(); }

    Action *Remove(Action *action);

    void PushBack(Action *action);
    void PushFront(Action *action);
    void InsertBefore(Action *action);
    void InsertAfter(Action *action);

    void ClearLane(int laneID);
    void clear() noexcept;

    bool empty() const noexcept;
    bool EmptyLane(int laneID) const noexcept;

    virtual void Update(float delta_t) override;

protected:
    std::vector<Action *> actions;
};
} // namespace tnt

#endif //!ACTIONS_HPP