#ifndef TNT_LIFETIME_MANAGER_HPP
#define TNT_LIFETIME_MANAGER_HPP

#include <algorithm>
#include <cassert>
#include <memory>

// utility header that manages the lifetime of different objects that depend on
// each-other.

namespace tnt
{
    namespace detail
    {
        class LifetimeTracker
        {
          public:
            LifetimeTracker(unsigned x) : duration{x} {}
            virtual ~LifetimeTracker() = 0;

            friend inline bool Compare(unsigned lifetime,
                                       LifetimeTracker const *rhs)
            {
                return (rhs->duration > lifetime);
            }

          private:
            unsigned duration;
        };

        inline LifetimeTracker::~LifetimeTracker() {}

        typedef LifetimeTracker **TrackerArray;
        extern TrackerArray LifeTracker;
        extern unsigned elements;

        template <typename T, typename Destroyer = std::default_delete<T>>
        class ConcreteLifetimeTracker : public LifetimeTracker
        {
          public:
            ConcreteLifetimeTracker(T *p, unsigned lifetime, Destroyer d)
                : LifetimeTracker{lifetime}, tracked{p}, destructor{d}
            {}
            ~ConcreteLifetimeTracker() { destructor(tracked); }

          private:
            T *tracked;
            Destroyer destructor;
        };
        static void AtExit()
        {
            assert(elements > 0 && LifeTracker != 0);
            LifetimeTracker *top{LifeTracker[elements - 1]};
            LifeTracker = static_cast<TrackerArray>(
                std::realloc(LifeTracker, sizeof(T) * (--elements)));
            delete top;
        }
    } // namespace detail

    template <typename T, typename Destroyer = std::default_delete<T>>
    void SetLifetime(T *dynObject, unsigned lifetime, Destroyer d)
    {
        detail::TrackerArray arr{static_cast<TrackerArray>(std::realloc(
            detail::LifeTracker, sizeof(T) * (detail::elements + 1)))};
        if (!arr)
            throw std::bad_alloc();
        detail::LifeTracker = arr;
        detail::LifetimeTracker *p{
            new detail::ConcreteLifetimeTracker<T, Destroyer>{dynObject,
                                                              lifetime, d}};
        detail::TrackerArray pos{std::upper_bound(
            detail::LifeTracker, detail::LifeTracker + detail::elements,
            lifetime, detail::Compare)};
        std::copy_backward(pos, detail::LifeTracker + detail::elements,
                           detail::LifeTracker + detail::elements + 1);
        *pos = p;
        ++detail::elements;
        std::atexit(detail::AtExit);
    }
} // namespace tnt

#endif //! TNT_LIFETIME_MANAGER_HPP