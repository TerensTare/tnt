#ifndef TNT_ECS_OBJECT_HPP
#define TNT_ECS_OBJECT_HPP

#include <map>
#include <typeindex>

#include "ecs/Component.hpp"
#include "utils/Logger.hpp"

namespace tnt
{
class Component;

/// @brief A class representing a basic game object.
class Object
{
public:
    /// @brief The destructor of the @em Object.
    virtual ~Object() noexcept;

    /// @brief Get the position of the @em Object.
    /// @return tnt::Vector
    Vector getPosition() const noexcept;

    /// @brief Set the position of the @em Object.
    /// @param pos The target position.
    void setPosition(Vector const &pos) noexcept;

    /// @brief Add @em pos to the current position.
    /// @param pos The  target position to be added.
    void Transform(Vector const &pos) noexcept;

    /// @brief Get the @em parent of the @em Object.
    /// @return tnt::Object*
    /// @note If the return is @em nullptr, it means that
    /// the @em Object has not @em parent.
    Object *getParent() const noexcept;

    /// @brief Set the @em parent of the @em Object.
    /// @param parent_ The new parent of the @em Object.
    void setParent(Object *parent_) noexcept;

    /// @brief Check if the @em Object should be updated.
    /// @return bool
    bool isActive() const noexcept;

    /// @brief Set the @em Object's @em active member equal to @em active_.
    /// @param active_ The new @em active status.
    void setActive(bool active_) noexcept;

    /// @brief Update the @em Object's stats. The function is pure virtual,
    /// meaning it should be implemented by the derived classed of @em Object.
    /// The function is meant to be run each frame, but you can get some speed
    /// by running it only if the @em Object is @em active.
    /// @param time_ The time elapsed since the last frame.
    virtual void Update([[maybe_unused]] long long time_) noexcept = 0;

    /// @brief Add a @em Component to the @em Object if it doesn't have
    /// it already and returns it. The function returns @em nullptr if @em T is
    /// not deriven from @em tnt::Component.
    /// @tparam T the type of @em Component desidered to be added to the @em Object.
    /// @return T*
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

    /// @overload This function has the same behaviour as the other
    /// add function, except that it needs variadic template, which
    /// are passed to the constructor of @em T.
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

    /// @brief Get the @em Component of type @em T from the
    /// @em components map of the @em Object. If @em T isn't
    /// on the @em components map, it returns @em nullptr.
    /// @tparam T The type of the @em Component you wish to get.
    /// @note The function is marked @em [[nodiscard]].
    /// @return T*
    template <typename T>
    [[nodiscard]] T *get() noexcept
    {
        auto key{std::type_index{typeid(T)}};
        if (components.find(key) != components.end())
            return static_cast<T *>(components[key]);
        tnt::logger::debug("Line: {}\tObject doesn't have Component {}\n Please call tnt::Object::add<T> before calling tnt::Object::get<T>!!", __LINE__, typeid(T).name());
        return nullptr;
    }

    /// @brief Remove a @em Component from the @em components
    /// map of the @em Object. It just logs if the @em Component
    /// doesn't exist on the map.
    /// @tparam T The type of the @em Component you want to remove
    /// from the @em Object.
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

    /// @brief Clears the @em components map of the @em Object.
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