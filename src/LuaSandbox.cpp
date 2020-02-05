// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Object.hpp"
#include "LuaSandbox.hpp"
#include "VirtualFileSystem.hpp"

sol::state tnt::LuaSandbox::lua = nullptr;

tnt::LuaSandbox::LuaSandbox()
{
    lua.open_libraries(sol::lib::package, sol::lib::base, sol::lib::math);

    loadVector();
    // loadMatrix();
    // loadVFS();
    loadObject();
    // loadTimer();
    // loadCamera();
}

tnt::LuaSandbox &tnt::LuaSandbox::This()
{
    static LuaSandbox l;
    return l;
}

void tnt::LuaSandbox::loadVector()
{
    lua.new_usertype<Vector>(
        "vector", sol::constructors<Vector(float, float)>(),
        "x", &Vector::x,
        "y", &Vector::y,

        "length_sqr", &Vector::MagnitudeSqr,
        "length", &Vector::Magnitude,
        "normalize", &Vector::Normalized,
        "rotate", &RotateVector,
        "nlerp", &nlerp,
        "dot", &Dot,

        sol::meta_function::addition, &operator+,
        sol::meta_function::subtraction, &operator-,
        sol::meta_function::division, &operator/,
        sol::meta_function::multiplication, &operator*,
        sol::meta_function::to_string, &operator<<,
        sol::meta_function::equal_to, &operator==,
        sol::meta_function::unary_minus, [](const Vector &v) {
            return Vector{-v.x, -v.y};
        });
}

void tnt::LuaSandbox::loadObject()
{
    lua.new_usertype<Object>(
        "object", sol::constructors<Object(Vector, float, Vector)>{},
        "pos", sol::property(&Object::getPosition, &Object::setPosition),
        "angle", sol::property(&Object::getAngle, &Object::setAngle),
        "scale", sol::property(&Object::getScale, &Object::setScale),
        "parent", sol::property(&Object::getParent, &Object::setParent));
}

// void tnt::LuaSandbox::loadMatrix()
// {
//     lua.new_usertype<Matrix>(
//         "matrix", sol::constructors<Matrix(unsigned, unsigned, double)>(),

//         "at", &Matrix::at,
//         "transpose", &Matrix::transpose,
//         "power_iter", &Matrix::power_iterator,
//         "deflation", &Matrix::deflation,

//         "rows", sol::property(&Matrix::Rows),
//         "columns", sol::property(&Matrix::Columns),

//         sol::meta_function::addition, &Matrix::operator+,
//         sol::meta_function::subtraction, &Matrix::operator-,
//         sol::meta_function::multiplication, &Matrix::operator*,
//         sol::meta_function::division, &Matrix::operator/);
// }

// void tnt::LuaSandbox::loadVFS()
// {
//     lua.new_usertype<VirtualFileSystem>(
//         "vfs",
//         "new", sol::no_constructor,
//         "this", &VirtualFileSystem::This,

//         "cwd", &VirtualFileSystem::CurrentDirectory,
//         "mount", &VirtualFileSystem::mount,
//         "unmount", &VirtualFileSystem::unmount,
//         "get", &VirtualFileSystem::get);
// }

void tnt::LuaSandbox::loadTimer() {}
void tnt::LuaSandbox::loadCamera() {}

tnt::LuaManager &tnt::LuaManager::This()
{
    static LuaManager inst;
    return inst;
}
