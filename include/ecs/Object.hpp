#ifndef TNT_ECS_OBJECT_HPP
#define TNT_ECS_OBJECT_HPP

#include <map>
#include <typeindex>

#include "math/Vector.hpp"
#include "utils/Concepts.hpp"
#include "utils/Logger.hpp"

// FIXME
// An Object can have two Component* of same type !!

// TODO:
// use concepts here for Component.

namespace tnt
{
class Component;

class Object
{
public:
    virtual ~Object() noexcept;

    Vector getPosition() const noexcept;
    void setPosition(Vector const &pos) noexcept;
    void Transform(Vector const &pos) noexcept;

    Object *getParent() const noexcept;
    void setParent(Object *parent_) noexcept;

    bool isActive() const noexcept;
    void setActive(bool active_) noexcept;

    virtual void Update([[maybe_unused]] long long time_) noexcept = 0;

    // template <typename T>
    // T *addComponent() noexcept
    // {
    //     if (auto key{std::type_index{typeid(T)}}, auto it{components.find(key)};
    //         it != components.end())
    //         components[key] = it.second;
    // }

    // template <typename T, typename... Args>
    // T *addComponent(Args &&... args) noexcept {}

    // template <typename T>
    // void removeComponent() noexcept {}

protected:
    bool active; // should be rendered (maybe) move this to Sprite ??
    Vector position;
    // std::pmr::map<std::type_index, Component *> components;
    Object *parent{nullptr};
};
} // namespace tnt

#endif //!TNT_ECS_OBJECT_HPP