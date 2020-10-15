#ifndef TNT_DOO_ECS_SCRIPTS_SYSTEM_HPP
#define TNT_DOO_ECS_SCRIPTS_SYSTEM_HPP

#include <sol/forward.hpp>
#include "doo_ecs/Base.hpp"

// TODO:
// find a way to support script reloading.

namespace tnt::doo
{
    /// @brief The system that handles the scripts of the objects.
    inline struct scripts_sys final
    {
        inline scripts_sys() noexcept = default;

        // scripts_sys(scripts_sys const &) = delete;
        // scripts_sys &operator=(scripts_sys const &) = delete;

        /// @brief The name of the *.lua script to attach to the next object.
        /// @param id The id of the object to add to the scripting system.
        /// @param filename The name of the .lua script file.
        void add_object(object const &id, std::string_view filename);

        /// @brief Run update() from the script.
        /// @param id The id of the object to update.
        /// @param time_ The elapsed time since the last update call.
        void Update(object const &id, float time_);

        /// @brief Load script data from a json chunk.
        /// @param id The id of the object to load from json.
        /// @param j The json chunk that holds the data.
        void from_json(object const &id, nlohmann::json const &j);

        /// @brief Draw widgets on the given window to modify the datas of the system.
        /// @param id The id of the active object.
        /// @param win The window where to draw the widgets.
        inline void draw_imgui(object const &id, Window const &win) noexcept {}

        std::vector<object> script_queue;    /// < The id-s of all the objects connected to scripts.
        std::vector<sol::state> states; /// < The lua handles of the objects.
    } scripts;                               /// < An instance of scripts_sys
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_SCRIPTS_SYSTEM_HPP