#ifndef TNT_ECS_OBJECT_HPP
#define TNT_ECS_OBJECT_HPP

#include <map>
#include <typeindex>

#include "ecs/Component.hpp"
#include "utils/Logger.hpp"

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

    template <typename T>
    T *add() noexcept
    {
        if constexpr (!std::is_base_of_v<Component, T>)
        {
            tnt::logger::debug("Line: {}\tCalling tnt::Object::add<T> with T being a type that is not derived from tnt::Component", __LINE__);
            return nullptr;
        }

        std::type_index key{typeid(T)};
        if (components.find(key) == components.end())
        {
            T *value{new T{}};
            components[key] = std::move(static_cast<Component *>(value));
        }

        return static_cast<T *>(components[key]);
    }

    template <typename T, typename... Args>
    T *add(Args &&... args) noexcept
    {
        if constexpr (!std::is_base_of_v<Component, T>)
        {
            tnt::logger::debug("Line: {}\tCalling tnt::Object::add<T> with T being a type that is not derived from tnt::Component", __LINE__);
            return nullptr;
        }

        std::type_index key{typeid(T)};
        if (components.find(key) == components.end())
        {
            T *value{new T{std::forward<Args>(args)...}};
            components[key] = std::move(static_cast<Component *>(value));
        }

        return static_cast<T *>(components[key]);
    }

    template <typename T>
    [[nodiscard]] T *get() noexcept
    {
        auto key{std::type_index{typeid(T)}};
        if (components.find(key) != components.end())
            return static_cast<T *>(components[key]);
        tnt::logger::debug("Line: {}\tObject doesn't have Component {}\n Please call tnt::Object::add<T> before calling tnt::Object::get<T>!!", __LINE__, typeid(T).name());
        return nullptr;
    }

    template <typename T>
    void remove() noexcept
    {
        auto key{std::type_index{typeid(T)}};
        if (auto it{components.find(key)}; it != components.end())
        {
            delete it.second;
            components.erase(it);
        }
        else
            tnt::logger::debug(
                "Line: {}\tObject doesn't have Component {}!!\nPlease call tnt::Object::add<T> before calling tnt::Object::remove<T>!!",
                __LINE__, typeid(T).name());
    }

    void clear() noexcept
    {
        for (auto const &it : components)
            if (it.second != nullptr)
                delete it.second;
        components.clear();
    }

protected:
    bool active; // should be rendered (maybe) move this to Sprite ??
    Vector position;
    std::map<std::type_index, Component *> components;
    Object *parent{nullptr};
};
} // namespace tnt

#endif //!TNT_ECS_OBJECT_HPP