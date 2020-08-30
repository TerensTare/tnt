#ifndef TNT_DOO_ECS_SCRIPTS_SYSTEM_HPP
#define TNT_DOO_ECS_SCRIPTS_SYSTEM_HPP

#include <sol/sol.hpp>
#include "doo_ecs/Base.hpp"

// TODO:
// support custom lua namespaces so that functions don't collide.

namespace tnt::doo
{
    /// @brief The system that handles the scripts of the objects.
    inline struct scripts_sys final
    {
        inline scripts_sys() noexcept = default;

        scripts_sys(scripts_sys const &) = delete;
        scripts_sys &operator=(scripts_sys const &) = delete;

        /// @brief The name of the *.lua script to attach to the next object.
        /// @param filename The name of the .lua script file.
        void add_object(std::string_view filename);

        /// @brief Run update() from the script.
        /// @param id The id of the object to update.
        /// @param time_ The elapsed time since the last update call.
        void Update(object const &id, float time_);

        /// @brief Load script data from a json chunk.
        void from_json(nlohmann::json const &j);

        std::vector<sol::protected_function> updates; /// < The script functions of the objects.
    } scripts;                                        /// < An instance of scripts_sys
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_SCRIPTS_SYSTEM_HPP