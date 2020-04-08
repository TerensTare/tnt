#ifndef ACTIONS_V2_HPP
#define ACTIONS_V2_HPP

#include <vector>
#include <functional>
#include "utils/Logger.hpp"

// TODO:
// find a way to make this more extensible.
// add ResumableAction.

// Code taken from
// https://github.com/TheMaverickProgrammer/Swoosh/blob/master/src/Swoosh/ActionList.h
namespace tnt
{
class Action
{
public:
    Action()
        : blocking{false}, done{false}, index{-1}, list{nullptr} {}

    virtual void update(float delta) = 0;

    void setAsFinished() noexcept { done = true; }
    const bool Done() const noexcept { return done; }
    const std::size_t Index() const noexcept { return index; }

protected:
    bool blocking;

private:
    friend class ActionList;

    friend class ClearPreviousActions;
    friend class ClearAllActions;
    friend class ConditionalBranchAction;
    friend class DelayAction;

    bool done;
    std::size_t index;
    ActionList *list;
};

class BlockingAction : public Action
{
public:
    BlockingAction() : Action() { blocking = true; }

    virtual void update(float delta) = 0;
};

class ClearPreviousActions;
class ClearAllActions;
class ConditionalBranchAction;
class DelayAction;

class ActionList
{
public:
    ActionList() : clearFlag{false} {}
    ~ActionList() noexcept { clear(); }

    void updateIndexes(std::size_t pos = 0) noexcept
    {
        for (pos; pos < items.size(); ++pos)
            ++items[pos]->index;
    }

    void insert(Action *item, std::size_t pos)
    {
        item->list = this;
        items.insert(items.begin() + pos, item);
        item->index = pos;
        updateIndexes(pos);
    }

    void insert(ActionList *other, std::size_t pos)
    {
        if (other == nullptr)
            logger::error("ActionList is empty!\n Line: {}, File: {}", __LINE__, __FILE__);

        for (int i{0}; i < other->items.size(); ++i)
            insert(other->items[i], pos + i);
        other->items.clear();
    }

    void add(Action *item)
    {
        item->list = this;
        items.push_back(item);
        item->index = items.size() - 1;
    }

    const bool empty() noexcept { return items.size(); }

    void clear() noexcept
    {
        for (auto it : items)
            delete it;
        items.clear();
        decltype(items){}.swap(items);
    }

    void append(ActionList *list)
    {
        if (list == nullptr)
            logger::error("ActionList is empty!\n Line: {}, File: {}", __LINE__, __FILE__);

        for (int i{0}; i < list->items.size(); ++i)
            items.push_back(list->items[i]);
        list->items.clear();
    }

    void update(float delta)
    {
        for (int i{0}; i < items.size();)
        {
            if (items[i]->Done())
            {
                delete items[i];
                items.erase(items.begin() + i);
                continue;
            }

            items[i]->index = std::size_t{i};
            items[i]->update(delta);

            if (clearFlag)
            {
                clearFlag = false;
                i = 0;
                continue;
            }

            if (items[i]->blocking)
                break;
            ++i;
        }
    }

private:
    std::vector<Action *> items;
    bool clearFlag;
};

class ClearPreviousActions : public BlockingAction
{
public:
    ClearPreviousActions() {}

    void update(float delta) override
    {
        if (Done())
            return;
        for (int i{0}; i < list->items.size();)
        {
            Action *item{list->items[i]};

            if (item != this)
            {
                delete item;
                list->items.erase(list->items.begin() + i);
                continue;
            }
            else
                break;
        }

        list->clearFlag = true;
        setAsFinished();
    }

private:
    friend class ActionList;
};

class ClearAllActions : public BlockingAction
{
public:
    ClearAllActions() {}

    void update(float delta) override
    {
        if (Done())
            return;
        for (int i{0}; i < list->items.size();)
        {
            Action *item{list->items[i]};
            if (item != this)
            {
                delete item;
                list->items.erase(list->items.begin() + i);
                continue;
            }
            ++i;
        }

        list->clearFlag = true;
        setAsFinished();
    }

private:
    friend class ActionList;
};

class ConditionalBranchAction : public BlockingAction
{
public:
    ConditionalBranchAction(std::function<bool()> condition,
                            ActionList *ifIsTrue, ActionList *ifIsFalse)
        : cond{condition}, ifTrue{ifIsTrue}, ifFalse{ifIsFalse} {}

    ~ConditionalBranchAction()
    {
        if (ifFalse)
        {
            delete ifFalse;
            ifFalse = nullptr;
        }
        if (ifTrue)
        {
            delete ifTrue;
            ifTrue = nullptr;
        }
    }

    virtual void update(double delta)
    {
        if (Done())
            return;

        if (cond())
        {
            list->insert(Index(), ifTrue);
            ifFalse->clear();
        }
        else
        {
            list->insert(Index(), ifFalse);
            ifTrue->clear();
        }

        setAsFinished();
    }

private:
    std::function<bool()> cond;
    ActionList *ifTrue, *ifFalse;

    friend class ActionList;
};

class DelayAction : public BlockingAction
{
public:
    explicit DelayAction(float duration)
        : elapsed{.0f}, total{duration} {}

    void update(float delta) noexcept override
    {
        if (Done())
            return;

        if (elapsed += delta; elapsed > total)
            setAsFinished();
    }

private:
    float elapsed;
    float total;

    friend class ActionList;
};
} // namespace tnt

#endif //!ACTIONS_V2_HPP