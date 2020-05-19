// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "utils/LuaManager.hpp"

#include "core/Input.hpp"
#include "core/Window.hpp"
#include "ecs/Sprite.hpp"
#include "fileIO/AssetManager.hpp"
// #include "fileIO/AudioPlayer.hpp"
#include "utils/Timer.hpp"

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

void tnt::lua::loadRectangle(sol::state_view lua_)
{
    lua_.new_usertype<Rectangle>(
        "rect", sol::constructors<Rectangle(float, float, float, float), Rectangle(int, int, int, int), Rectangle(Vector const &, float, float), Rectangle(Rectangle)>{},
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
            sol::resolve<Rectangle(Rectangle const &, float const &)>(&tnt::operator*)),

        sol::meta_function::equal_to, sol::resolve<bool(Rectangle const &, Rectangle const &)>(&tnt::operator==),

        sol::meta_function::to_string, sol::resolve<std::ostream &(std::ostream &, Rectangle const &)>(&tnt::operator<<),
        sol::meta_function::unary_minus, [](Rectangle const &r) {
            return Rectangle{-r.x, -r.y, -r.w, -r.h};
        });
}

void tnt::lua::loadTimer(sol::state_view lua_)
{
    lua_.new_usertype<Timer>(
        "timer", sol::default_constructor,
        "start", &Timer::start, "reset", &Timer::reset, "stop", &Timer::stop,
        "paused", &Timer::paused, "delta", &Timer::deltaTime);
}

// void tnt::lua::loadAssetManager(sol::state_view lua_)
// {
//     lua_.new_usertype<AssetManager>(
//         "assets", "new", sol::no_constructor, "this", &AssetManager::This,
//         "add_font", &AssetManager::AddFont, "add_image", &AssetManager::AddImage,
//         "add_music", &AssetManager::AddMusic, "add_sfx", &AssetManager::AddSfx,
//         "image", &AssetManager::Image, "font", &AssetManager::Font,
//         "music", &AssetManager::Music, "sfx", &AssetManager::Sfx);
// }

// void tnt::lua::loadAudioPlayer(sol::state_view lua_)
// {
//     lua_.new_usertype<AudioPlayer>(
//         "audio", sol::constructors<AudioPlayer(int, unsigned short, int, int) noexcept>{},
//         "play", &AudioPlayer::PlayMusic,
//         "pause", &AudioPlayer::PauseMusic,
//         "resume", &AudioPlayer::ResumeMusic,
//         "play_sfx", &AudioPlayer::PlaySFX);
// }

// this is needed to bind tnt::Object to Lua. Update() cannot be
// called directly from LuaObject, so no undesired stuff happens.

class LuaObject : public tnt::Object
{
private:
    void Update(long long) noexcept override { return; }
};

void tnt::lua::loadObject(sol::state_view lua_)
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

// void tnt::lua::loadComponents(sol::state_view lua_)
// {
//     lua_.new_usertype<SpriteComponent>(
//         "sprite_comp", sol::constructors<SpriteComponent(Window const *, std::string_view), SpriteComponent(Window const *, std::string_view, Rectangle const &)>{},
//         "get_texture", &SpriteComponent::getTexture,
//         "set_texture",
//         sol::overload(
//             sol::resolve<void(Window const *, std::string_view)>(&SpriteComponent::setTexture),
//             sol::resolve<void(Window const *, std::string_view, Rectangle const &)>(&SpriteComponent::setTexture)),
//         "w", &SpriteComponent::getWidth,
//         "h", &SpriteComponent::getHeight);
// }

// class LuaSprite final : public tnt::Sprite
// {
// private:
//     void Update(long long) noexcept override { return; }
// };

// void tnt::lua::loadSprite(sol::state_view lua_)
// {
//     lua_.new_usertype<LuaSprite>(
//         "sprite", sol::constructors<Sprite(Window const *, std::string_view, float), Sprite(Window const *)>{}
//         // "get_sprite", &LuaSprite::getSprite
//     );
// }

void tnt::lua::loadInput(sol::state_view lua_)
{
    auto input{lua_["input"].get_or_create<sol::table>()};
    input["close_input"] = &input::close;

    // keyboard stuff
    input["key_down"] = &input::keyDown;
    input["key_pressed"] = &input::keyPressed;
    input["key_released"] = &input::keyReleased;
    input["last_key"] = &input::lastKeyPressed;

    // mouse stuff
    input["button_down"] = &input::mouseButtonDown;
    input["button_pressed"] = &input::mouseButtonPressed;
    input["button_released"] = &input::mouseButtonReleased;
    input["last_button"] = &input::lastMouseButton;

    input["update"] = &input::updateCurrent;
    input["update_last"] = &input::updatePrevious;
}

void tnt::lua::loadWindow(sol::state_view lua_)
{
    lua_.new_usertype<Window>(
        "window", sol::constructors<Window(std::string_view, int, int, int, int, Uint32), Window(std::string_view, int, int)>{},

        "clear", &Window::Clear, "render", &Window::Render,
        "set_clear_color", sol::resolve<void(Uint8, Uint8, Uint8, Uint8)>(&Window::setClearColor),

        "opened", &Window::isOpened, "handleEvents", &Window::handleEvents,

        "draw",
        sol::overload(
            &Window::Draw<Sprite, Camera>, &Window::Draw<Sprite, HorizontalCamera>,
            &Window::Draw<Sprite, FullTrackingCamera>));
}

void tnt::lua::loadAll(sol::state_view lua_)
{
    loadVector(lua_);
    loadRectangle(lua_);
    loadTimer(lua_);
    // loadAudioPlayer(lua_);
    loadObject(lua_);
    // loadSprite(lua_);
    loadInput(lua_);
    loadWindow(lua_);
    // loadAssetManager(lua_);
    // loadComponents(lua_);
}