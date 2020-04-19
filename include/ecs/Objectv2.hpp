#ifndef TNT_ECS_OBJECT_V2_HPP
#define TNT_ECS_OBJECT_V2_HPP

#include <map>
#include <typeindex>

#include "math/Vector.hpp"
#include "utils/Logger.hpp"

// TODO:
// use concepts here for Component.

namespace tnt
{
    class Component;

    // thx r/koctogon
    // https://www.reddit.com/r/cpp_questions/comments/g37onh/is_this_a_possible_logical_thing_to_do/
    class Object
    {
      public:
        virtual ~Object() noexcept { clearComponents(); }

        template <typename T> void addComponent() noexcept(components[getType<T>()])
        {
            if (components[getType<T>()].second != nullptr)
                return;
            components[getType<T>()].first = [ty = getType<T>()] {
                delete static_cast<T *>(ty.second);
                ty.second = nullptr;
            };
            components[getType<T>()].second = new T{};
        }

        template <typename T, typename... Args>
        void addComponent(Args &&... args) noexcept(components[getType<T>()])
        {
            if (components[getType<T>()].second != nullptr)
                return;
            components[getType<T>()].first = [ty = getType<T>()] {
                delete static_cast<T *>(ty.second);
                ty.second = nullptr;
            };
            components[getType<T>()].second = new T{args...};
        }

        template <typename T> T *getComponent() noexcept(components[getType<T>()])
        {
            if (components[getType<T>()].second == nullptr)
                tnt::logger::debug(
                    "Calling Object::getComponent<T>() when T isn't a Component of this Object!!");
            return static_cast<T *>(components[getType<T>()].second);
        }

        template <typename T> void removeComponent() noexcept(components[getType<T>()])
        {
            if (components[getType<T>()] == nullptr)
                return;
            components[getType<T>()].first();
            components.erase(getType<T>());
        }

        void clearComponents() noexcept
        {
            for (auto const &it : components) it.second.first();
            components.clear();
        }

        virtual void Update(long long const &time_) noexcept = 0;

      protected:
        std::map<std::type_index, std::pair<void (*)(), Component *>> components;
        Vector position;

      private:
        template <typename T> auto getType() noexcept { return std::type_index{typeid(T)}; }
    };
} // namespace tnt

#endif //! TNT_ECS_OBJECT_V2_HPP