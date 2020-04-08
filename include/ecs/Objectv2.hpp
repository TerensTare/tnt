#ifndef TNT_ECS_OBJECT_V2_HPP
#define TNT_ECS_OBJECT_V2_HPP

// NOTE: dangerous experiments happen here.
#include <utility>
#include "utils/TypeLists.hpp"

namespace tnt
{
class Object
{
public:
    template <typename T, typename... Args>
    void addComponent(Args &&... args) noexcept
    {
        if (tl::index_v<Components, T> == -1) // object doesn't have that Component.
        {
            T *t{new T{std::forward<Args...>(args)}};
        }
    }

    template <typename T>
    T *getComponent() noexcept;

private:
    using Components = TypeList<NullType, NullType>;
};
} // namespace tnt

#endif //!TNT_ECS_OBJECT_V2_HPP