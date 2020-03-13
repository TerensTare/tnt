#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <memory>
#include <vector>
#include <algorithm>
#include "Concepts.hpp"

// TODO: Physics : public Observable.
namespace tnt
{
class Observer
{
public:
    virtual ~Observer() noexcept {}
    virtual void Update(Observable *obj) = 0;
};

class Observable
{
public:
    virtual ~Observable()
    {
        for (auto it : observers)
            Detach(it.lock());
    }

    void Attach(std::shared_ptr<Observer> o)
    {
        bool exists{false};
        for (auto it{observers.begin()}; it != observers.end(); ++it)
            if (it->lock() == o)
            {
                exists = true;
                break;
            }
        if (!exists)
            observers.emplace_back(o);
    }

    void Detach(std::shared_ptr<Observer> o)
    {
        observers.erase(
            std::remove_if(
                observers.begin(), observers.end(),
                [&](std::weak_ptr<Observer> const &ptr) {
                    return (ptr.expired() || (ptr.lock() == o));
                }),
            observers.end());
    }

    void Notify() noexcept
    {
        for (auto o : observers)
            o.lock()->Update(this);
    }

private:
    std::vector<std::weak_ptr<Observer>> observers;
};
} // namespace tnt

#endif //!OBSERVER_HPP