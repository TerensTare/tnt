// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "exp/LuaManager.hpp"

#include "math/Vector.hpp"
#include "ecs/Object.hpp"

void tnt::lua::loadVector(sol::state &lua_)
{
    lua_.new_usertype<Vector>(
        "vector", sol::constructors<Vector(float, float), Vector(Vector const &)>(),
        "x", &Vector::x, "y", &Vector::y,

        "length_sqr", &Vector::MagnitudeSqr, "length", &Vector::Magnitude,
        "normalize", &Vector::Normalized, "rotate", &RotateVector,
        "nlerp", &nlerp, "dot", &Dot,

        sol::meta_function::addition, sol::resolve<Vector(Vector const &, Vector const &)>(&tnt::operator+),
        sol::meta_function::subtraction, sol::resolve<Vector(Vector const &, Vector const &)>(&tnt::operator-),
        sol::meta_function::division, sol::resolve<Vector(Vector const &, float const &)>(&tnt::operator/),
        sol::meta_function::multiplication, sol::resolve<Vector(Vector const &, float const &)>(&tnt::operator*),

        sol::meta_function::equal_to, sol::resolve<bool(Vector const &, Vector const &)>(&tnt::operator==),
        sol::meta_function::less_than, sol::resolve<bool(Vector const &) const>(&tnt::Vector::operator<),
        sol::meta_function::less_than_or_equal_to, sol::resolve<bool(Vector const &, Vector const &)>(&tnt::operator<=),

        sol::meta_function::to_string, sol::resolve<std::ostream &(std::ostream &, Vector const &)>(&tnt::operator<<),
        sol::meta_function::unary_minus, [](const Vector &v) {
            return Vector{-v.x, -v.y};
        });

    // (maybe) enum these ??
    lua_["VECTOR_ZERO"] = VECTOR_ZERO;
    lua_["VECTOR_ONE"] = VECTOR_ONE;
    lua_["VECTOR_RIGHT"] = VECTOR_RIGHT;
    lua_["VECTOR_LEFT"] = VECTOR_LEFT;
    lua_["VECTOR_UP"] = VECTOR_UP;
    lua_["VECTOR_DOWN"] = VECTOR_DOWN;
}

// this is needed to bind tnt::Object to Lua. Update() cannot be
// called directly from LuaObject, so no undesired stuff happens.

class LuaObject : public tnt::Object
{
private:
    void Update(long long time_) override { return; }
};

void tnt::lua::loadObject(sol::state &lua_)
{
    lua_.new_enum("coords",
                  "local", LuaObject::LOCAL,
                  "global", LuaObject::GLOBAL);

    lua_.new_usertype<LuaObject>(
        "object", sol::no_constructor,

        "angle", sol::property([](LuaObject const &obj) { return obj.getAngle(LuaObject::LOCAL); }, &LuaObject::setAngle),
        "gAngle", [](LuaObject const &obj) { return obj.getAngle(LuaObject::GLOBAL); },
        "rotate", &LuaObject::Rotate,

        "scale", sol::property([](LuaObject const &obj) { return obj.getScale(LuaObject::LOCAL); }, &LuaObject::setScale),
        "gScale", [](LuaObject const &obj) { return obj.getScale(LuaObject::GLOBAL); },
        "rescale", &LuaObject::Scale,

        "pos", sol::property([](LuaObject const &obj) { return obj.getPosition(LuaObject::LOCAL); }, &LuaObject::setPosition),
        "gPos", [](LuaObject const &obj) { return obj.getPosition(LuaObject::GLOBAL); },
        "transform", &LuaObject::Transform,

        "parent", sol::property(&LuaObject::getParent, &LuaObject::setParent),
        "active", sol::property(&LuaObject::isActive, &LuaObject::setActive));
}