// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "utils/LuaManager.hpp"

#include "core/Input.hpp"
#include "core/Window.hpp"
#include "core/Space.hpp"
#include "core/Scene.hpp"

#include "doo_ecs/Objects.hpp"

#include "ecs/Sprite.hpp"
#include "ecs/RigidBody.hpp"

#include "fileIO/AssetManager.hpp"
// #include "fileIO/AudioPlayer.hpp"

#include "imgui/ImGui.hpp"

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
    lua_["LEFT"] = VECTOR_LEFT;
    lua_["UP"] = VECTOR_UP;
    lua_["DOWN"] = VECTOR_DOWN;
}

void tnt::lua::loadRectangle(sol::state_view lua_)
{
    lua_.new_usertype<Rectangle>(
        "rect", sol::constructors<Rectangle(), Rectangle(float, float, float, float), Rectangle(int, int, int, int), Rectangle(Vector const &, float, float), Rectangle(Rectangle)>{},
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
//     sol::table Update(long long) noexcept override { return; }
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
    input["close"] = &input::close;

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

void tnt::lua::loadImGui(sol::state_view lua_)
{
    auto imgui{lua_["imgui"].get_or_create<sol::table>()};

    imgui.new_enum("win_flags",
                   "colapsible", ImGui::WindowFlags::Collapsible,
                   "closable", ImGui::WindowFlags::Closable,
                   "resizable", ImGui::WindowFlags::Resizable,
                   "movable", ImGui::WindowFlags::Movable,
                   "with_titlebar", ImGui::WindowFlags::WithTitleBar,
                   "opaque_bg", ImGui::WindowFlags::OpaqueBackground,
                   "widget_first", ImGui::WindowFlags::WidgetThenText);

    imgui["init"] = &tnt_imgui_init;
    imgui["update"] = &ImGui::update_context;
    imgui["close"] = &tnt_imgui_close;

    imgui["Begin"] = sol::overload(
        [](Window const *win, std::string_view name, int x, int y) -> bool {
            return ImGui::Begin(win, name, x, y);
        },
        &ImGui::Begin);
    imgui["End"] = &ImGui::End;

    imgui["begin_section"] = &ImGui::BeginSection;
    imgui["end_section"] = &ImGui::EndSection;

    imgui["begin_list"] = &ImGui::BeginList;
    imgui["list_item"] = &ImGui::list_item;
    imgui["end_list"] = &ImGui::EndList;

    imgui["begin_menubar"] = &ImGui::BeginMenuBar;
    imgui["menu_button"] = &ImGui::menu_button;
    imgui["menu_item"] = &ImGui::menu_item;
    imgui["end_menubar"] = &ImGui::EndMenuBar;

    imgui["button"] = &ImGui::button;

    imgui["slider_int"] = &ImGui::slider_int;
    imgui["slider_float"] = &ImGui::slider_float;

    imgui["hslider_int"] = &ImGui::hslider_int;
    imgui["hslider_float"] = &ImGui::hslider_float;
    imgui["hslider_int2"] = &ImGui::hslider_int2;
    imgui["hslider_float2"] = &ImGui::hslider_float2;
    imgui["hslider_vec"] = &ImGui::hslider_vec;

    imgui["checkbox"] = &ImGui::checkbox;

    imgui["progress_bar"] = &ImGui::progress_bar;
    imgui["newline"] = &ImGui::newline;

    imgui["text"] = &ImGui::text;
    imgui["colored_text"] = &ImGui::colored_text;
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

void tnt::lua::loadSpace(sol::state_view lua_)
{
    lua_.new_usertype<Space>(
        "space", sol::constructors<Space()>{},
        "active", sol::property(&Space::isActive),
        "update", &Space::Update, "draw",
        sol::overload(
            &Space::Draw<Camera>,
            &Space::Draw<HorizontalCamera>, &Space::Draw<FullTrackingCamera>),
        "add", &Space::addObject,
        "get", &Space::getObject,
        "remove", &Space::removeObject);
}

void tnt::lua::loadScene(sol::state_view lua_)
{
    lua_.new_usertype<Scene>(
        "scene", sol::constructors<Scene(Window const *, std::string_view) noexcept, Scene(Window const *) noexcept>{},
        "add", &Scene::addSpace, "get", &Scene::getSpace,
        "draw", &Scene::Draw, "update", &Scene::Update);
}

void tnt::lua::loadSpriteComp(sol::state_view lua_)
{
    lua_.new_usertype<SpriteComponent>(
        "sprite_comp",
        sol::constructors<SpriteComponent(Window const *, std::string_view), SpriteComponent(Window const *, std::string_view, tnt::Rectangle const &)>{},
        "setTexture",
        sol::overload(
            sol::resolve<void(Window const *, std::string_view)>(
                &SpriteComponent::setTexture),
            sol::resolve<void(Window const *, std::string_view, tnt::Rectangle const &)>(
                &SpriteComponent::setTexture)),
        "draw", &SpriteComponent::Draw,
        "w", &SpriteComponent::getWidth,
        "h", &SpriteComponent::getHeight);
}

void tnt::lua::loadPhysComp(sol::state_view lua_)
{
    lua_.new_usertype<PhysicsComponent>(
        "phys_comp", sol::constructors<PhysicsComponent(float const &, Vector const &, Vector const &)>{},
        "mass", sol::property(&PhysicsComponent::getMass, &PhysicsComponent::setMass),
        "vel", sol::property(&PhysicsComponent::getVelocity),
        "accel", sol::property(&PhysicsComponent::getAcceleration),
        "max_vel", &PhysicsComponent::getMaxVelocity,
        "max_accel", &PhysicsComponent::getMaxAcceleration,
        "dir", &PhysicsComponent::getDirection,
        "speed", &PhysicsComponent::getSpeed,
        "max_speed", &PhysicsComponent::getMaxSpeed,
        "apply", &PhysicsComponent::applyForce,
        "do_phys", &PhysicsComponent::doPhysics);
}

void tnt::lua::loadDooEcs(sol::state_view lua_)
{
    using namespace doo;

    lua_.new_usertype<object_data>(
        "object_data", sol::constructors<object_data(float, tnt::Vector, tnt::Vector)>{},
        "angle", &object_data::angle,
        "scale", &object_data::scale,
        "pos", &object_data::pos);

    // sprites
    lua_.new_usertype<sprite_comp>(
        "sprite_comp",
        sol::constructors<sprite_comp(tnt::Window const &, std::string_view), sprite_comp(tnt::Window const &, std::string_view, tnt::Rectangle const &)>{},
        "crop", &sprite_comp::crop);

    lua_.new_usertype<sprites_sys>(
        "sprites_sys", sol::constructors<sprites_sys()>{},
        "add_object",
        sol::overload(
            sol::resolve<void(tnt::Window const &, std::string_view)>(&sprites_sys::add_object),
            sol::resolve<void(tnt::Window const &, std::string_view, tnt::Rectangle const &)>(&sprites_sys::add_object)),
        "draw_queue", &sprites_sys::draw_queue);

    // physics
    lua_.new_usertype<phys_comp>(
        "phys_comp", sol::constructors<phys_comp(float const &, tnt::Vector const &)>{},
        "inv_mass", &phys_comp::inv_mass,
        "vel", &phys_comp::vel, "accel", &phys_comp::accel);

    lua_.new_usertype<physics_sys>(
        "physics_sys", sol::constructors<physics_sys()>{},
        "add_object", &physics_sys::add_object,
        "add_force", &physics_sys::addForce,
        "update", &physics_sys::Update,
        "phys", &physics_sys::phys);

    lua_.new_usertype<objects_sys>(
        "objects_sys", sol::constructors<objects_sys()>{},
        "add_object", &objects_sys::add_object,
        "update", &objects_sys::Update,
        "draw", &objects_sys::Draw,
        "get_data", &objects_sys::get_data,
        "active", &objects_sys::active);

    lua_["sprites"] = sprites;
    lua_["physics"] = physics;
    lua_["objects"] = objects;
}

void tnt::lua::loadAll(sol::state_view lua_)
{
    loadVector(lua_);
    loadRectangle(lua_);
    loadTimer(lua_);
    loadObject(lua_);
    loadInput(lua_);
    loadImGui(lua_);
    loadWindow(lua_);
    loadSpace(lua_);
    loadScene(lua_);
    loadSpriteComp(lua_);
    loadPhysComp(lua_);
    // loadAssetManager(lua_);
    // loadAudioPlayer(lua_);
    // loadSprite(lua_);
}

void tnt::lua::load(sol::state_view lua_, std::span<tnt::lua::lib> libs)
{
    for (auto l : libs)
    {
        if (l == lib::ALL)
        {
            loadAll(lua_);
            return;
        }
        else if (l == lib::MATH)
        {
            loadVector(lua_);
            loadRectangle(lua_);
        }
        else if (l == lib::TIMER)
            loadTimer(lua_);
        else if (l == lib::INPUT)
            loadInput(lua_);
        else if (l == lib::WINDOW)
            loadWindow(lua_);
        else if (l == lib::IMGUI)
            loadImGui(lua_);
        else if (l == lib::CAMERA)
            loadCameras(lua_);
        else if (l == lib::SCENE)
        {
            loadSpace(lua_);
            loadScene(lua_);
        }
        else if (l == lib::SPRITE_COMP)
            loadSpriteComp(lua_);
        else if (l == lib::PHYS_COMP)
            loadPhysComp(lua_);
        else if (l == lib::DOO_ECS)
            loadDooEcs(lua_);
        else if (l == lib::UTILS)
            loadUtils(lua_);
    }
}