// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <sol/sol.hpp>

#include "math/Rectangle.hpp"
#include "tolua/LuaMath.hpp"

void tnt::lua::loadVector(sol::state_view lua_)
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
        sol::meta_function::multiplication, sol::resolve<Vector(Vector const &, float const &)>(&tnt::operator*)); //,

    // sol::meta_function::equal_to, sol::resolve<bool(Vector const &, Vector const &)>(&tnt::operator==),
    // sol::meta_function::less_than, [](Vector const &lhs, Vector const &rhs) -> bool { return lhs < rhs; },
    // sol::meta_function::less_than_or_equal_to, [](Vector const &lhs, Vector const &rhs) -> bool { return lhs <= rhs; });

    // (maybe) enum these ??
    lua_["VECTOR_ZERO"] = VECTOR_ZERO;
    lua_["VECTOR_ONE"] = VECTOR_ONE;
    lua_["VECTOR_RIGHT"] = VECTOR_RIGHT;
    lua_["LEFT"] = VECTOR_LEFT;
    lua_["UP"] = VECTOR_UP;
    lua_["DOWN"] = VECTOR_DOWN;
}

void tnt::lua::loadRect(sol::state_view lua_)
{
    lua_.new_usertype<Rectangle>(
        "rect", sol::constructors<Rectangle(), Rectangle(float, float, float, float), Rectangle(int, int, int, int), Rectangle(Vector const &, float, float), Rectangle(Rectangle const &)>{},
        "x", &Rectangle::x, "y", &Rectangle::y, "w", &Rectangle::w, "h", &Rectangle::h,
        "point_in", &Rectangle::Contains, "point_out", &Rectangle::Outside,

        sol::meta_function::addition,
        sol::overload(
            sol::resolve<Rectangle(Rectangle const &, Rectangle const &)>(&tnt::operator+),
            sol::resolve<Rectangle(Rectangle const &, Vector const &)>(&tnt::operator+)),

        sol::meta_function::subtraction,
        sol::overload(
            sol::resolve<Rectangle(Rectangle const &, Rectangle const &)>(&tnt::operator-),
            sol::resolve<Rectangle(Rectangle const &, Vector const &)>(&tnt::operator-)),

        sol::meta_function::division,
        sol::overload(
            sol::resolve<Rectangle(Rectangle const &, Vector const &)>(&tnt::operator/),
            sol::resolve<Rectangle(Rectangle const &, float const &)>(&tnt::operator/)),
        sol::meta_function::multiplication,
        sol::overload(
            sol::resolve<Rectangle(Rectangle const &, Vector const &)>(&tnt::operator*),
            sol::resolve<Rectangle(Rectangle const &, float const &)>(&tnt::operator*)));
}