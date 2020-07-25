#ifndef TNT_ECS_SCRIPTABLE_HPP
#define TNT_ECS_SCRIPTABLE_HPP

#include "ecs/Object.hpp"

namespace tnt
{
    /// @brief A @c Component that has a behaviour defined in a Lua script.
    class Script final : public Component
    {
    public:
        /// @brief @c Script 's constructor.
        /// @param filename Name of the script file.
        explicit Script(std::string_view filename) noexcept;

        /// @brief Run certain functions located on the script file.
        /// @param time_ The elapsed time since the last call.
        void Update(float time_) noexcept;

    private:
        std::string file;
    };
} // namespace tnt

#endif //!TNT_ECS_SCRIPTABLE_HPP