#ifndef TNT_IMPORT_DATA_FROM_LUA_HPP
#define TNT_IMPORT_DATA_FROM_LUA_HPP

#include <span>
#include <sol/sol.hpp>

#include "core/Config.hpp"
#include "utils/BitFlags.hpp"

namespace tnt::lua
{
    /// @brief An enum containing all of the engine's structures/functions that can be exported.
    /// @todo Add support for easings and numerical springing.
    enum class lib : uint8_t
    {
        vector = 1 << 0,               /// < Export @ref tnt::Vector.
        rect = 1 << 1,                 /// < Export @ref tnt::Rectangle.
        window = 1 << 2,               /// < Export @ref tnt::Window.
        input = 1 << 3,                /// < Export namespace @ref tnt::input.
        audio = 1 << 4,                /// < Export @ref tnt::AudioManager.
        imgui = 1 << 5,                /// < Export @ref tnt::ImGui.
        utils = 1 << 6,                /// < Export @ref tnt::sparse_set.
        core = window | input | audio, /// < Export @ref tnt::Window, @ref tnt::input and @ref tnt::AudioManager.
        math = vector | rect,          /// < Export @ref tnt::Vector, @ref tnt::Rectangle.
    };

    TNT_API void load(sol::state_view lua_, tnt::lua::lib const &libs);
} // namespace tnt::lua

namespace tnt
{
    template <>
    struct enable_bit_mask<lua::lib> : std::true_type
    {
    };
} // namespace tnt

#endif //!TNT_IMPORT_DATA_FROM_LUA_HPP