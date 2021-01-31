#ifndef TNT_DOO_ECS_SCRIPTS_SYSTEM_HPP
#define TNT_DOO_ECS_SCRIPTS_SYSTEM_HPP

#include <sol/sol.hpp>
#include <nlohmann/json_fwd.hpp>

#include "core/Window.hpp"
#include "doo_ecs/Base.hpp"
#include "tolua/LuaLoader.hpp"
#include "types/SparseSet.hpp"

// TODO:
// find a way to support script reloading.
// remove unnecessary #include-s.
// load custom Lua bindings for each objects from json or code.

namespace tnt::lua
{
    NLOHMANN_JSON_SERIALIZE_ENUM(
        lib, {{lib::audio, "audio"},
              {lib::core, "core"},
              {lib::imgui, "imgui"},
              {lib::input, "input"},
              {lib::math, "math"},
              {lib::rect, "rect"},
              {lib::utils, "utils"},
              {lib::vector, "vector"},
              {lib::window, "window"}})
}

namespace tnt::doo
{
    /// @brief The system that handles the scripts of the objects.
    inline struct scripts_sys final
    {
        /// @brief The name of the *.lua script to attach to the next object.
        /// @param id The id of the object to add to the scripting system.
        /// @param filename The name of the .lua script file.
        /// @param libs The desired libraries you need for this object. Defaults to @ref tnt::lua::lib::core.
        void add_object(object const &id, std::string_view filename);

        /// @brief Run `init()` from the object's lua script. If no function named init() is found, it does nothing.
        /// @param id The id of the object to init().
        void Init(object const &id);

        /// @brief Reload the script data of the desired object.
        /// @param id The id of the desired object.
        void reload(object const &id) noexcept;

        /// @brief Call the desired function from the lua state of the given object.
        /// @param id The id of the object that has the function.
        /// @param fn The name of the function.
        /// @param args The arguments to pass to the function.
        template <typename... Args>
        inline decltype(auto) call(object const &id, std::string_view fn, Args &&... args)
        {
            sol::safe_function fun{states[id][fn].get_or_create<sol::safe_function>()};
            if (auto const &res = fun(std::forward<Args>(args)...);
                res.valid())
                return res.get();
            else
            {
                sol::error const &err{res};
                logger::error(err.what());
            }
        }

        /// @brief Run update() from the script.
        /// @param id The id of the object to update.
        /// @param time_ The elapsed time since the last update call.
        void Update(object const &id, float time_);

        /// @brief Load script data from a json chunk.
        /// @param id The id of the object to load from json.
        /// @param j The json chunk that holds the data.
        void from_json(object const &id, nlohmann::json const &j);

        /// @brief Store scripts data of a specific object to a json chunk.
        /// @param id The id of the object to serialize to json.
        /// @param j The json chunk where the data will be saved.
        void to_json(object const &id, nlohmann::json &j) ;

        /// @brief Draw widgets on the given window to modify the datas of the system.
        /// @param id The id of the active object.
        /// @param win The window where to draw the widgets.
        inline void draw_imgui(object const &id, Window const &win) noexcept {}

        /// @brief Remove the desired object from the scripts system.
        /// @param id The id of the object you want to remove.
        void remove(object const &id) noexcept;

        /// @brief Remove all the objects from the scripts system.
        void clear() noexcept;

        tnt::sparse_set<object> active;      /// < The id-s of all the objects connected to scripts.
        std::vector<sol::state> states; /// < The lua handles of the objects.
    } scripts;                          /// < An instance of scripts_sys
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_SCRIPTS_SYSTEM_HPP