// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <sol/sol.hpp>

#include "doo_ecs/Animations.hpp"
#include "doo_ecs/Bones.hpp"
#include "doo_ecs/Objects.hpp"
#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Sprites.hpp"
#include "doo_ecs/Steering.hpp"

#include "tolua/LuaDooEcs.hpp"

void tnt::lua::loadDooEcs(sol::state_view lua_)
{
    using namespace doo;

    // TODO: cameras
    // TODO(maybe): remove Draw/Update/draw_imgui functions ??

    lua_.new_usertype<object_data>(
        "object_data",
        sol::constructors<
            object_data(
                float, tnt::Vector const &, tnt::Vector const &) noexcept,
            object_data(float, tnt::Vector const &, tnt::Vector const &, object const &) noexcept>{},
        "angle", &object_data::angle,
        "scale", &object_data::scale,
        "pos", &object_data::pos);

    lua_.new_usertype<objects_sys>(
        "objects_sys",
        "new", sol::no_constructor,
        "add_object", [](objects_sys &, object_data const &o) { return objects.add_object(o); },
        "get_data", [](objects_sys &, object const &id) { return objects.get_data(id - 1); },
        "active", &objects_sys::active,
        "parent", &objects_sys::parent,
        "set_parent", &objects_sys::set_parent,

        "remove", [](objects_sys &, object const &id) { objects.remove(id - 1); },
        "clear", &objects_sys::clear,

        "angle", &objects_sys::angle,
        "gAngle", [](objects_sys &, object const &id) { return objects.gAngle(id - 1); },
        "scale", &objects_sys::scale,
        "gScale", [](objects_sys &, object const &id) { return objects.gScale(id - 1); },
        "pos", &objects_sys::pos,
        "gPos", [](objects_sys &, object const &id) { return objects.gPos(id - 1); });

    // sprites
    lua_.new_usertype<sprite_comp>(
        "sprite_comp",
        sol::constructors<
            sprite_comp(tnt::Window const &, std::string_view),
            sprite_comp(
                tnt::Window const &, std::string_view, tnt::Rectangle const &)>{},
        "crop", &sprite_comp::crop);

    lua_.new_usertype<sprites_sys>(
        "sprites_sys",
        "new", sol::no_constructor,
        "add_object", [](sprites_sys &, object const &id, sprite_comp const &s) { sprites.add_object(id - 1, s); },
        "draw", [](sprites_sys &, object const &id, Window const &win) { sprites.Draw(id - 1, win); },
        "draw_area", [](sprites_sys &, object const &id) { return sprites.draw_area(id - 1); },

        "remove", [](sprites_sys &, object const &id) { sprites.remove(id - 1); },
        "clear", &sprites_sys::clear,

        "active", &sprites_sys::active,
        "clip", &sprites_sys::clip);

    // physics
    lua_.new_usertype<physics_comp>(
        "phys_comp",
        sol::factories([](body_type b, float m, float d, float r, Vector const &mv, Vector const &ma, Rectangle const &bound) { return physics_comp{b, m, d, r, mv, ma, bound}; },
                       [](body_type b, float m, float d, float r, Rectangle const &bound) {
                           return physics_comp{.type{b}, .mass{m}, .damping{d}, .restitution{r}, .bound_box{bound}};
                       }),
        "type", &physics_comp::type,
        "mass", &physics_comp::mass,
        "damping", &physics_comp::damping,
        "restitution", &physics_comp::restitution,
        "max_vel", &physics_comp::maxVel,
        "max_accel", &physics_comp::maxAccel,
        "bound_box", &physics_comp::bound_box);

    lua_.new_usertype<physics_sys>(
        "physics_sys",
        "new", sol::no_constructor,
        "add_object", [](physics_sys &, object const &id, physics_comp const &p) { physics.add_object(id - 1, p); },
        "add_force", [](physics_sys &, object const &id, Vector const &f) { physics.addForce(id - 1, f); },
        "add_glob_force", &physics_sys::addGlobalForce,
        "update", [](physics_sys &, object const &id, float dt) { physics.Update(id - 1, dt); },
        "colliding", [](physics_sys &, object const &id, object const &id2) { return physics.colliding(id - 1, id2 - 1); },
        "resolve", [](physics_sys &, object const &id, object const &id2) { physics.resolve(id - 1, id2 - 1); },

        "remove", [](physics_sys &, object const &id) { physics.remove(id - 1); },
        "clear", &physics_sys::clear,

        "inv_mass", &physics_sys::inv_mass,
        "vel", &physics_sys::vel,
        "gVel", [](physics_sys &, object const &id) { return physics.gVel(id - 1); },
        "max_vel", &physics_sys::maxVel,
        "gMaxVel", [](physics_sys &, object const &id) { return physics.gMaxVel(id - 1); },
        "accel", &physics_sys::accel,
        "gAccel", [](physics_sys &, object const &id) { return physics.gAccel(id - 1); },
        "max_accel", &physics_sys::maxAccel,
        "gMaxAccel", [](physics_sys &, object const &id) { return physics.gMaxAccel(id - 1); },
        "damping", &physics_sys::damping,
        "restitution", &physics_sys::restitution,
        "active", &physics_sys::active,
        "bound", &physics_sys::bound_box);

    // animations
    lua_.new_enum("anim_wrap",
                  "single_run", animation_comp::wrap_mode::single_run,
                  "loop", animation_comp::wrap_mode::loop);

    lua_.new_enum("anim_dir",
                  "horizontal", animation_comp::direction::horizontal,
                  "vertical", animation_comp::direction::vertical);

    lua_.new_usertype<animation_comp>(
        "anim_comp",
        sol::constructors<animation_comp(Rectangle const &,
                                         int const, float const, float const,
                                         animation_comp::direction const &,
                                         animation_comp::wrap_mode const &) noexcept>{});

    lua_.new_usertype<animations_sys>(
        "animations_sys",
        "new", sol::no_constructor,
        "add_object", [](animations_sys &, object const &id, animation_comp const &a) { animations.add_object(id - 1, a); },
        "update", [](animations_sys &, object const &id, float dt) { animations.Update(id - 1, dt); },

        "remove", [](animations_sys &, object const &id) { objects.remove(id - 1); },
        "clear", &animations_sys::clear,

        "wrap", &animations_sys::wrap,
        "dir", &animations_sys::dir,
        "startX", &animations_sys::startX,
        "startY", &animations_sys::startY,
        "speed", &animations_sys::speed,
        "elapsed", &animations_sys::elapsed,
        "timePerFrame", &animations_sys::timePerFrame,
        "spacing", &animations_sys::spacing,
        "current", &animations_sys::current,
        "running", &animations_sys::active);

    lua_.new_usertype<bone_comp>(
        "bone_comp", sol::factories([](Vector const &joint, float const length) noexcept {
            return bone_comp{.joint{joint}, .length{length}};
        }),
        "joint", &bone_comp::joint,
        "length", &bone_comp::length);

    lua_.new_usertype<bones_sys>(
        "bones_sys", "new", sol::no_constructor,

        "add_object", [](bones_sys &, object const &id, bone_comp const &b) { bones.add_object(id - 1, b); },
        "remove", [](bones_sys &, object const &id) { bones.remove(id - 1); },
        "clear", &bones_sys::clear,
        "follow", [](bones_sys &, object const &id, Vector const &p) { bones.follow(id - 1, p); },

        "active", &bones_sys::active,
        "joint", &bones_sys::joint,
        "length", &bones_sys::length);

    lua_.new_usertype<steering_sys>(
        "steering_sys", "new", sol::no_constructor,

        "seek", [](steering_sys &, object const &id, Vector const &dst) { steer.Seek(id - 1, dst); },
        "flee", [](steering_sys &, object const &id, Vector const &dst) { steer.Flee(id - 1, dst); },
        "ranged_flee", [](steering_sys &, object const &id, Vector const &dst, float const r) { steer.RangedFlee(id - 1, dst, r); },
        "arrive", [](steering_sys &, object const &id, Vector const &dst) { steer.Arrive(id - 1, dst); },
        "pursuit", [](steering_sys &, object const &id, object const &id2) { steer.Pursuit(id - 1, id2 - 1); },
        "evade", [](steering_sys &, object const &id, object const &id2) { steer.Evade(id - 1, id2 - 1); });

    lua_["objects"] = &objects;
    lua_["sprites"] = &sprites;
    lua_["physics"] = &physics;
    lua_["animations"] = &animations;
    lua_["steer"] = &steer;
    lua_["bones"] = &bones;
}