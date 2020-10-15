// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <sol/sol.hpp>

#include "doo_ecs/Animations.hpp"
#include "doo_ecs/Objects.hpp"
#include "doo_ecs/Physics.hpp"
#include "doo_ecs/Sprites.hpp"

#include "tolua/LuaDooEcs.hpp"

void tnt::lua::loadDooEcs(sol::state_view lua_)
{
    using namespace doo;

    // TODO: animations, cameras, steering
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
        "objects_sys", sol::factories([] { return &objects; }),
        "add_object", &objects_sys::add_object,
        "get_data", &objects_sys::get_data,
        "active", &objects_sys::active,
        "parent", &objects_sys::parent,
        "set_parent", &objects_sys::set_parent,

        "angle", &objects_sys::angle,
        "gAngle", &objects_sys::gAngle,
        "scale", &objects_sys::scale,
        "gScale", &objects_sys::gScale,
        "pos", &objects_sys::pos,
        "gPos", &objects_sys::gPos);

    // sprites
    lua_.new_usertype<sprite_comp>(
        "sprite_comp",
        sol::constructors<
            sprite_comp(tnt::Window const &, std::string_view),
            sprite_comp(
                tnt::Window const &, std::string_view, tnt::Rectangle const &)>{},
        "crop", &sprite_comp::crop);

    lua_.new_usertype<sprites_sys>(
        "sprites_sys", sol::factories([] { return &sprites; }),
        "add_object", &sprites_sys::add_object,
        "draw", &sprites_sys::Draw,
        "draw_area", &sprites_sys::draw_area,

        "draw_queue", &sprites_sys::draw_queue,
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
        "physics_sys", sol::factories([] { return &physics; }),
        "add_object", &physics_sys::add_object,
        "add_force", &physics_sys::addForce,
        "add_glob_force", &physics_sys::addGlobalForce,
        "update", &physics_sys::Update,
        "colliding", &physics_sys::colliding,
        "resolve", &physics_sys::resolve,

        "inv_mass", &physics_sys::inv_mass,
        "vel", &physics_sys::vel,
        "gVel", &physics_sys::gVel,
        "max_vel", &physics_sys::maxVel,
        "gMaxVel", &physics_sys::gMaxVel,
        "accel", &physics_sys::accel,
        "gAccel", &physics_sys::gAccel,
        "max_accel", &physics_sys::maxAccel,
        "gMaxAccel", &physics_sys::gMaxAccel,
        "damping", &physics_sys::damping,
        "restitution", &physics_sys::restitution,
        "active", &physics_sys::physics_queue,
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
        "animations_sys", sol::factories([] { return &animations; }),
        "add_object", &animations_sys::add_object,
        "update", &animations_sys::Update,

        "wrap", &animations_sys::wrap,
        "dir", &animations_sys::dir,
        "startX", &animations_sys::startX,
        "startY", &animations_sys::startY,
        "speed", &animations_sys::speed,
        "elapsed", &animations_sys::elapsed,
        "timePerFrame", &animations_sys::timePerFrame,
        "spacing", &animations_sys::spacing,
        "current", &animations_sys::current,
        "running", &animations_sys::running);

    lua_["objects"] = &objects;
    lua_["sprites"] = &sprites;
    lua_["physics"] = &physics;
    lua_["animations"] = &animations;
}