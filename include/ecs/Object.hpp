#ifndef TNT_ECS_OBJECT_HPP
#define TNT_ECS_OBJECT_HPP

#include <map>
#include <typeindex>

#include "math/Vector.hpp"
#include "utils/Logger.hpp"
#include "ecs/Base.hpp"

// TODO:
// AnimationComponent creates a new SDL_Texture on it's constructor. Fix that!!
// Remove the need to call new/delete for each tnt::Component added to tnt::Object.

// TODO(maybe):
// use a weak_ptr on SpriteComponent ??
// rename AnimationComponent to just Animation ??
// PhysicsComponent::{add/remove}Mass ??
// Rename every component to a shorter name, ex. *Comp ??
// Write a Polygon class so that PhysicsComponent can use it as collision_shape ??
// remove all getters/setters and use Components like C-style structures or POD./ ??
// Serializable<T>/concept ??
// Systems: DrawSys, PhysicsSys(/CollisionSys), AnimationSys, AISys ??

namespace tnt
{
    /// @brief A class representing a basic game object.
    class Object
    {
    public:
        /// @brief The types of Object coordinates.
        enum Coordinates
        {
            LOCAL = 0, ///< Local coordinates (related to itself)
            GLOBAL = 1 ///< Global coordinates (related to the parent)
        };

        /// @brief The destructor of the @c Object.
        virtual ~Object() noexcept;

        /// @brief Get the position of the @c Object.
        /// @param coords The type of coordinates, global by default.
        /// @return tnt::Vector
        Vector getPosition(Coordinates const &coords = Coordinates::GLOBAL) const noexcept;

        /// @brief Set the position of the @c Object.
        /// @param pos The target position.
        void setPosition(Vector const &pos) noexcept;

        /// @brief Add @c pos to the current position.
        /// @param pos The target position to be added.
        void Transform(Vector const &pos) noexcept;

        /// @brief Set the angle of this @c Object equal to the @c radian parameter.
        /// @param radian The angle in radian to be set as the value for this @c Object.
        /// @note The angle passed as a parameter is considered to be in radians.
        void setAngle(float radian) noexcept;

        /// @brief Get the angle that this @c Object holds.
        /// @param coords The type of coordinates, global by default.
        /// @return float
        /// @note The returned angle is in radian.
        float getAngle(Coordinates const &coords = Coordinates::GLOBAL) const noexcept;

        /// @brief Add @c radian to the current angle of the @c Object.
        /// @param radian The angle to be added.
        /// @note The angle passed as a parameter is considered to be in radians.
        void Rotate(float radian) noexcept;

        /// @brief Set the scale of this @c Object to be equal to @c ratio.
        /// @param ratio The scale to be set as a value to this @c Object.
        void setScale(Vector const &ratio) noexcept;

        /// @brief Get the scale of this @c Object.
        /// @param coords The type of coordinates, global by default.
        /// @return tnt::Vector
        Vector getScale(Coordinates const &coords = Coordinates::GLOBAL) const noexcept;

        /// @brief Add @c ratio to the current scale of this @c Object.
        /// @param ratio The scale to be added.
        void Scale(Vector const &ratio) noexcept;

        /// @brief Get the @c parent of the @c Object.
        /// @return tnt::Object*
        /// @note If the return is @c nullptr, it means that
        /// the @c Object has not @c parent.
        Object *getParent() const noexcept;

        /// @brief Set the @c parent of the @c Object.
        /// @param parent_ The new parent of the @c Object.
        void setParent(Object *parent_) noexcept;

        /// @brief Check if the @c Object should be updated.
        /// @return bool
        bool isActive() const noexcept;

        /// @brief Set the @c Object's @c active member equal to @c active_.
        /// @param active_ The new @c active status.
        void setActive(bool active_) noexcept;

        /// @brief Update the @c Object's stats. The function is pure virtual,
        /// meaning it should be implemented by the derived classed of @c Object.
        /// The function is meant to be run each frame, but you can get some speed
        /// by running it only if the @c Object is @c active.
        /// @param time_ The time elapsed since the last frame.
        virtual void Update([[maybe_unused]] long long time_) noexcept = 0;

        /// @brief Function to check if @c Object has a @c Component without
        /// causing any errors.
        /// @tparam The type of the @c Component to check on the components map.
        template <component T>
        bool has() const
            noexcept(noexcept((components.find(typeid(T)) != components.cend())))
        {
            return (components.find(typeid(T)) != components.cend());
        }

        /// @brief Add a @c Component to the @c Object if it doesn't have
        /// it already and returns it. The function returns @c nullptr if @c T is
        /// not deriven from @c tnt::Component.
        /// @tparam T the type of @c Component desidered to be added to the @c Object.
        /// @return T*
        template <component T>
        T *add() noexcept
        {
            std::type_index key{typeid(T)};
            [[likely]] if (!has<T>())
            {
                T *value{new T{}};
                components[key] = std::move(static_cast<Component *>(value));
            }

            return static_cast<T *>(components[key]);
        }

        /// @overload This function has the same behaviour as the other
        /// add function, except that it needs variadic template, which
        /// are passed to the constructor of @c T.
        template <component T, typename... Args>
        T *add(Args &&... args) noexcept
        {
            std::type_index key{typeid(T)};
            [[likely]] if (!has<T>())
            {
                T *value{new T{std::forward<Args>(args)...}};
                components[key] = std::move(static_cast<Component *>(value));
            }

            return static_cast<T *>(components[key]);
        }

        /// @brief Get the @c Component of type @c T from the
        /// @c components map of the @c Object. If @c T isn't
        /// on the @c components map, it returns @c nullptr.
        /// @tparam T The type of the @c Component you wish to get.
        /// @note The function is marked @c [[nodiscard]].
        /// @return T*
        template <component T>
        [[nodiscard]] T *get() const noexcept
        {
            const std::type_index key{typeid(T)};
            [[likely]] if (has<T>()) return static_cast<T *>(components.at(key));
            tnt::logger::debug("Object doesn't have Component {}\n"
                               "Please call tnt::Object::add<T> before "
                               "calling tnt::Object::get<T>!!",
                               key.name());
            return nullptr;
        }

        /// @brief Remove a @c Component from the @c components
        /// map of the @c Object. It just logs if the @c Component
        /// doesn't exist on the map.
        /// @tparam T The type of the @c Component you want to remove
        /// from the @c Object.
        template <component T>
        void remove() noexcept
        {
            [[likely]] if (has<T>())
            {
                delete it.second;
                it = components.erase(it);
            }
            [[unlikely]] else tnt::logger::debug(
                "Object doesn't have Component {}!!"
                "\nPlease call tnt::Object::add<T> before calling"
                " tnt::Object::remove<T>!!",
                typeid(T).name());
        }

        /// @brief Clears the @c components map of the @c Object.
        inline void clear() noexcept
        {
            for (auto const &it : components)
                [[likely]] if (it.second != nullptr) delete it.second;
            components.clear();
        }

    protected:
        bool active{true}; // should be rendered (maybe) move this to Sprite ??
        float angle{.0f};
        Vector position{.0f, .0f};
        Vector scale{1.f, 1.f};
        Object *parent{nullptr};
        std::map<std::type_index, Component *> components;
    };

} // namespace tnt

#endif //!TNT_ECS_OBJECT_HPP