#ifndef TNT_IMPORT_DATA_FROM_LUA_HPP
#define TNT_IMPORT_DATA_FROM_LUA_HPP

#include <span>
#include <sol/sol.hpp>
#include "core/Config.hpp"

namespace tnt::lua
{
    /// @brief An enum containing all of the engine's structures/functions that can be exported.
    /// @todo Separate math into vector and rectangle, and so on for the others.
    /// @todo Add support for easings and numerical springing.
    enum class lib : uint32_t
    {
        math, /// < Export @ref tnt::Vector, @ref tnt::Rectangle.
        core, /// < Export @ref tnt::Window, @ref tnt::input and @ref tnt::AudioManager.
        imgui, /// < Export @ref tnt::ImGui.
        utils /// < Export @ref tnt::sparse_set.
    };

    TNT_API void load(sol::state_view lua_, std::span<const tnt::lua::lib> libs);
} // namespace tnt::lua

#endif //!TNT_IMPORT_DATA_FROM_LUA_HPP