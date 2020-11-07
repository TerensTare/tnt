#ifndef TNT_EXPORT_UTILS_TO_LUA
#define TNT_EXPORT_UTILS_TO_LUA

#include <sol/forward.hpp>
#include "core/Config.hpp"

namespace tnt::lua
{
    TNT_API void loadSparseSet(sol::state_view lua_);
}

#endif // !TNT_EXPORT_UTILS_TO_LUA