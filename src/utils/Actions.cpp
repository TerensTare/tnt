// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "utils/Actions.hpp"

tnt::Action::Action(bool blocking)
    : blocks{blocking}, finished{false} {}

void tnt::ActionList::Update(float delta_t)
{
    std::vector<Action *>::iterator it{actions.begin()};
    int mask{0};

    while (it != actions.end())
    {
        Action *action{*it};
        if (0 == (mask & action->Lanes()))
        {
            if (action->Blocks())
                mask |= action->Lanes();
            action->Update(delta_t);

            if (action->Finished())
            {
                action->End();
                it = actions.erase(it);
            }
            else
                ++it;
        }
        else
            ++it;
    }
}

tnt::ActionList::~ActionList() noexcept
{
    for (auto it{actions.begin()}; it != actions.end(); ++it)
        if (*it != nullptr)
        {
            (*it)->Cancel();
            delete *it;
            *it = nullptr;
        }
    actions.clear();
}

void tnt::ActionList::clear() noexcept
{
    actions.clear();
}

bool tnt::ActionList::empty() const noexcept
{
    return actions.empty();
}

void tnt::Sync::Update(float delta_t)
{
    if (*owner.begin() == this)
        finished = true;
}

tnt::Delay::Delay(float time)
    : elapsed{.0f}, duration{time}, Action{true} {}

void tnt::Delay::Update(float delta_t)
{
    elapsed += delta_t;
    if (elapsed > duration)
        finished = true;
}

void tnt::HierarchicalAction::Update(float delta_t)
{
    owner.Update(delta_t);
    finished = owner.empty();
}