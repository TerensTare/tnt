#ifndef TNT_MAIN_HEADER_HPP
#define TNT_MAIN_HEADER_HPP

#include "async/CoUtils.hpp"

#include "core/Config.hpp"
#include "core/Input.hpp"
#include "core/Window.hpp"

#include "doo_ecs/Animations.hpp"
#include "doo_ecs/Bones.hpp"
#include "doo_ecs/Cameras.hpp"
#include "doo_ecs/Objects.hpp"
#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Scripts.hpp"
#include "doo_ecs/Sprites.hpp"
#include "doo_ecs/Steering.hpp"

#include "fileIO/AssetCache.hpp"
#include "fileIO/AudioPlayer.hpp"
#include "fileIO/Snipper.hpp"
#include "fileIO/TextureAtlas.hpp"
#include "fileIO/VirtualFS.hpp"

#include "format/FormatedRectangle.hpp"

#include "imgui/gui_config.hpp"
#include "imgui/ImGui.hpp"

#include "json/JsonRectangle.hpp"

#include "math/Easings.hpp"
#include "math/Rectangle.hpp"
#include "math/Springing.hpp"

#include "mirror/Mirror.hpp"

#include "pcg/Random.hpp"
#include "pcg/Noise.hpp"

#include "tolua/LuaCore.hpp"
#include "tolua/LuaDooEcs.hpp"
#include "tolua/LuaImGui.hpp"
#include "tolua/LuaLoader.hpp"
#include "tolua/LuaMath.hpp"

#include "types/Containers.hpp"
#include "types/Expected.hpp"
#include "types/PolyResource.hpp"
#include "types/ScopeUtils.hpp"
#include "types/Sort.hpp"
#include "types/Timer.hpp"

#include "utils/Assert.hpp"
#include "utils/Benchmark.hpp"
#include "utils/BitFlags.hpp"
#include "utils/Logger.hpp"
#include "utils/Traits.hpp"
#include "utils/TypeUtils.hpp"

#endif // !TNT_MAIN_HEADER_HPP