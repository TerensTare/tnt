# Changelog

All changes to this project will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## 2020-05-25
### Added
-`tnt::ImGui::hslider_int2` and `tnt::ImGui::hslider_float2`.

### Removed
- ID-related macros for ImGui.
- `core/Graphics.hpp`/`.cpp`.


## 2020-05-23
### Added
- `tnt::AssetManager` now automatically converts relative path to absolute. That includes even `tnt::SpriteComponent`.
- `tnt::Camera` constructor that accepts `tnt::Rectangle`.
- `tnt::Sprite::Draw` which needs only a `tnt::Window const*`.
- An experimental implementation of `tnt::Object`.
- A partial implementation of animations for `drawable` `tnt::Object`s. Not fully finished/tested yet.
- Check for window resizing on `tnt::Window::handleEvents`.

### Fixed
- `tnt::Object` rotating incorrectly.
- `tnt::drawable` not showing when using `tnt::Space`.
- `tnt::ImGui::hslider_*` not drawing correctly and/or out of bounds.

### Changed
- `main.cpp` is now a basic work in progress editor for the engine.
- `tnt::ImGui::hslider_*` now draw their value's text on top of the thumb, for a cleaner look.
- Tried to optimize `tnt::Space::Draw`.
- Most `tnt::Object` members are initialized by using default member initalizers.
- `tnt::logger::*` functions that accept a formatting string and one argument, now accept `T&&` instead of `T`.

## 2020-05-19
### Added
- `tnt::Script`, a `tnt::Component` which can be used with Lua scripts.
- Attributes `[[likely]]` and `[[unlikely]]` `on tnt::Object`'s `template` functions.
- Utility function `tnt::Object::has<T>` to check if the object has the specified `tnt::Component` without errors.

### Changed
- Some stuff on the implementation of `tnt::Space`.
- Header `Component.hpp` is deprecated and will be removed on another commit. `class tnt::Component` and `concept tnt::component` are now defined on `Object.hpp`. `tnt::Script` and `tnt::SpriteComponent` are now defined to `Scriptable.hpp/.cpp` and `Sprite.hpp/.cpp`.
- `tnt::file::Snipper::onModify` now uses `concept`s.
- `utilities` folder is now renamed to `extra`.

### Removed
- `math/Matrix.hpp` and `math/Matrix.cpp`.

## 2020-05-18
### Changed
- `LuaManager` functions now take `sol::state_view` as an argument rather than a `sol::state&`. Also a default parameter is passed to them.

### Removed
- `LuaSandbox.hpp/.cpp` because it was unused and `LuaManager` is a better alternative to it.

## 2020-05-17
### Added
- `tnt::input::lastKeyPressed`.
- More Lua bindings. Now `tnt::Window` can be used with Lua. However, a small number of it's member functions is available.

### Changed
- `LuaManager.hpp` is now on folder `utils`. Please use it instead of `LuaSandbox.hpp`.
- `lua` and `sol2` are now required to build the project.
- Tried to optimize `tnt::input::mouse...` functions.
- `concept tnt::drawable` now correctly detects drawable objects.
- `tnt::Window::Draw` is now redesigned. Also removed the overload which drawed a `tnt::SpriteComponent`.

## 2020-05-16
### Added
- Lua bindings for some engine types.

### Changed
- `tnt::AssetManager` functions now use `const_iterator`s.
- ImGui functions now don't copy window instances passed to them.

### Removed
- `tnt::window_ref` and `tnt::window_ptr` on [core/Window.hpp](./include/core/Window.hpp).
- Unnecessary checks on `tnt::AssetManager` destructor.

## 2020-05-15
### Added
- TnT is now live on Discord. Come [visit](https://discord.gg/s5Y9VCR), ask questions and suggest anything anytime.
- `operator const SDL_Rect() const` and `operator const SDL_FRect() const` for `tnt::Rectangle`.

### Changed
- Made some optimizations on `tnt::SpriteComponent::Draw` by minimizing copies.

### Removed
- Redundant line on `CMakeLists.txt`.

## 2020-05-14
### Added
- Modularized version of `utils` headers on folder `modules`.
- Some documentation on some header files.
- `concept drawable` at the end of `Object.hpp`.

### Changed
- Moved `concept component` to `Component.hpp`.
- Renamed `tnt::SizeOfArray` lambda to `tnt::array_size` and used a custom `concept` to make it work only with arrays.

### Removed
- File `Concepts.hpp`.

## 2020-05-13
### Changed
- Updated `concept camera` to detect camera types correctly. Moved it to `Camera.hpp`.

## 2020-05-11
### Added
- New `tnt::Window` constructor, which omits position and flags for a simpler construction.
- `tnt::Window::isOpened` and `tnt::Window::handleEvents`. Note that these functions should be used together, because using `tnt::Window::isOpened` as a condition for the game loop without calling `tnt::Window::handleEvents` after polling events may cause an infinite loop.
- New ImGui widget `colored_text` which draws a text with a specified color.
- `const` on some ImGui widget members.
- `explicit` specifier on `tnt::Action` and `tnt::BlockingAction` `tnt::stack_allocator` constructors.

### Changed
- `tnt::Object` methods now handle data related to global and local coordinates.

### Removed
- Widget `menu` on ImGui. Please use `tnt::ImGui::BeginMenuBar`, `tnt::ImGui::EndMenuBar`, `tnt::ImGui::menu_button` and `tnt::ImGui::menu_item` instead.
- `tnt::RotateComponent` and `tnt::ScaleComponent` to be part of `tnt::Object`.

## 2020-05-10
### Added
- Horizontal ImGui sliders now will display the current value on their center.

### Changed
- All ImGui slider widgets (vertical and horizontal) don't require an ID, x or y position, instead they require just a text to display on them.
- `hslider_int` and `hslider_float` now require an extra `text` as the second parameter.

## 2020-05-08
### Added
- New implementation of `Action`s, together with the documentation. Also a test on folder `test/actions` is included, showing the basic usage of `Action`s.
- Section `Documentation` on `README.md`.
- `FindSphinx.cmake` on `cmake` folder in case of starting to use `Sphinx` for documentation.

### Removed
- Experimental implementations of `Action`s. Please use the new implementation instead.
- `Easingsv2.hpp` from experimental folder. Use the original implementation on folder `math` instead.

## 2020-05-07
### Added
- New CMake `option` `TNT_BUILD_DOCS` to build the documentation using Doxygen. Note that the switch is `ON` by default.

### Removed
- `docs` folder. The website is now hosted on the `gh-pages` branch of the repository.


## 2020-05-06
### Added
- `tnt::Window::operator SDL_Window*` is now `explicit`.

### Removed
- `Debug` build for AppVeyor.

## 2020-05-05
### Added
- Documentation for some header files.
- New overload for `tnt::SpriteComponent::setTexture` which should be used for clipped textures.
- Check on `tnt::Snipper::onModify` for `typename Func`.
- Function `tnt::Snipper::unwatchFile()`.

### Changed
- `tnt::SpriteComponent::getWidth()`/`tnt::SpriteComponent::getHeight()` now returns the correct width if the texture is clipped.
- `tnt::SpriteComponent`'s destructor is no longer `virtual`.


## 2020-05-04
### Added
- Build status for Azure Pipelines builds.
- `INSTALL.md` to learn how to obtain the binaries of the engine.

### Changed
- Renamed `InputManager.hpp` and `InputManager.cpp` to `Input.hpp` and `Input.cpp`.
- Moved `Random.hpp` to the correct folder.
- CMake's minimum version now is `3.12.4` because C++20 support is required to build the project.


## 2020-05-03
### Added
- `inline` on some `struct Vector` functions.
- `AppVeyor` build and it's status badge on `README.md`.
- `stack_allocator<T>` can now be used in production code.
- Prefix `TNT_` on `Allocator.hpp`'s include guard.
- Example/test showing the usage of `stack_allocator<T>` with `struct tnt::Vector`.

## Removed
- `tnt::Vector& operator=(tnt::Vector&&)`.

## 2020-05-02
### Added
- Some project-related stuff on CMakeLists.txt. The project should compile as always because the build process isn't affected by these changes.
- `inline`, `const` and `noexcept` on some `class Expected<T>` functions.
- Better check on `tnt::ImGui::button()`/`tnt::ImGui::text()`/`tnt::ImGui::list_item()` to not draw if outside window.
- Template functions for `class Object` to attach or remove `Component`s.
- New test `dynamic_ecs` for `class Object`'s new functions.
- `TNT_` prefix for `Logger.hpp` include guard.
- Stuff to turn `Logger.hpp` into a C++20 module partition.
- `TnT.ixx`, which will be used as the main file of the module TnT.

### Changed
- Fixed a typo related to `SDL2_DIR` on `azure-pipelines.yml`.
- Every `class` derived from `class Component` is (and should be) `final`, derived ONLY from `clas Component` and NOT `virtual`-ly inherited.
- Moved the `class Timer` test to folder `test/timer/`.

### Removed
- `#include <SDL2/SDL.h>` on [Component.hpp](./include/ecs/Component.hpp).

## 2020-04-29
### Added
- Conditional check before `delete`-ing an `Object`'s `parent`.
- Ability to move and resize ImGui windows. Widgets won't be drawn if they won't fit on the window.

### Changed
- Renamed `concept camera_type` to `concept camera`.
- Renamed `StackAllocator` to `stack_allocator`. It is still not functional.
- Fixed `#include` on `AudioPlayer.cpp`.

## 2020-04-27
### Added
- Code Inspector badge on [README.md](./README.md).
- `class Rectangle`, `class AssetManager` loader functions on [LuaManager.hpp](./include/exp/LuaManager.hpp).
- `final` keyword for `class AssetManager`.
- `noexcept` on some `struct Vector` functions.

### Changed
- Method of comparing `struct Vector`-s. That shouldn't affect your code, though.


## 2020-04-26
### Added
- `noexcept` on some `struct Camera` functions.
- Widget `text` and `newline` for ImGui.

## 2020-04-25
### Changed
- `tnt::ImGui::menu()` now needs a `std::string_view*` rather than `std::string*` to conserve space.
- Now ImGui widgets require `std::size_t` for their id rather than `long long`, so that the id is not negative and it is compatible with `std::hash`.
- The title on ImGui's windows is now more centered.
- `tnt::ImGui::checkbox()` now doesn't need to have an `id`, `x` and `y`, because it is drawn on the window. Just a `window` where to draw it, the `text` to draw to it and a `bool *value`. The same with `tnt::ImGui::button()` and `tnt::ImGui::menu()`.
- `tnt::ImGui::progress_bar()` now has an offset between it's body and it's filled part and doesn't need `x` and `y` as parameter, but needs a `text` to draw on its side.
- `tnt::ImGui::button()`'s width is now dependent on the size of the text you want to display on it.
- Fixed a bug with `tnt::ImGui::button()` showing the wrong color.
- Made the text size of `tnt::ImGui::button()` to be equal to the global font size.
- The font ImGui used from `zeldadx` to `Inconsolata`.
- The slider color of `hslider` and `slider` ImGui widgets.


## 2020-04-24
### Added
- Functions `tnt::ImGui::BeginList()/EndList()` and `tnt::ImGui::list_item()` for indexed and unindexed lists.
- Some spacing before the first item of `tnt::ImGui::menu()`.
- Check on `tnt::ImGui::menu()` so that you don't add two menu widgets on the same window.

### Removed
- `long long id` from widget `tnt::ImGui::progress_bar`.

## 2020-04-23
### Added
- Started working on lists and indexed lists for ImGui.
- Check on `CMakeLists.txt` to delete `assets` directory on the `build/type` folder if already exists.
- New assets [tick.png](./assets/tick.png) to be used on `checkbox` widget for ImGui.

### Changed
- The way `tnt::ImGui::menu` worked to get equal spacing between different options.
- ImGui's `menu` widget spacing from 50 to 20.

## 2020-04-22
### Added
- Some files to be compiled on `CMakeLists.txt`.
- `bool active` + getter/setter on `class tnt::Object`.
- Some utiltity functions on [Space.hpp](./include/core/Space.hpp).
- `randomFloat()` on [pcg/Random.hpp](./include/pcg/Random.hpp).
- Implementation of `Camera::Shake()`.

### Changed
- Updated [.gitignore](./.gitignore).
- Moved Camera.hpp/cpp to folder `core`.
- Moved AudioPlayer.hpp/cpp to folder `fileIO`.
- `Camera::Bounds()` to return a `tnt::Rectangle`.
- Renamed `Camera::Screenshake()` to just `Camera::Shake()`.

### Removed
- `Camera::FBounds()`.

## 2020-04-21
### Added
- Widget `checkbox` for ImGui.

### Changed
- The way `Runtimer.hpp` works.

### Removed
- Animation-related stuff on `class Sprite`.

## 2020-04-20
### Added
- `concept component`.
- assets\ prefix for places where assets are required.
- Linking with SDL2_ttf on `CMakeLists.txt`.
- Check on `CMakeLists.txt` for linking SDL2 on the correct way on different platforms.

### Changed
- Moved `.clang-format` and `.editorconfig` to new folder `utilities`.
- `ret` on [Timer.hpp](./include/utils/Timer.hpp) from `auto` to `static auto` to display correct fps.

### Removed
- Animation related functions on `class Sprite`.
- `Objectv2.hpp`, because the problem it was trying to solve could be solved without templates.

## 2020-04-19
### Added
- Some functions on Objectv2.hpp/.cpp.
- Command `CMakeLists.txt` to copy all assets to the binary directory.

### Changed
- `vcpkg.exe` to just vcpkg on `CMakeLists.txt` for cross-platform building.

### Removed
- `assets/zeldadxt.png` because it is unnecessary.

## 2020-04-18
### Added
- [CONTRIBUTING.md](./CONTRIBUTING.md).
- Some logging messages on `tnt::ImGui::Begin()` and `tnt::ImGui::End()`.
- A call to `tnt::input::updatePrevious()` and `tnt::input::updateCurrent()` on `tnt::ImGui::update_context()`.

### Changed
- Contributing section of [README.md](./README.md#L91) to point to [CONTRIBUTING.md](./CONTRIBUTING.md).
- Some lines on CMakeLists.txt which don't affect building.

### Removed
- Some redundant lines on `tnt::ImGui::End()`.
- scipts/format-proj.ps1 and scripts/ttf2png.py.

## 2020-04-15
### Added
- azure-pipelines.yml.
- Added option on ImGui to load colored text.
- tnt_imgui_init() and tnt_imgui_close(). Now you just need to call them instead of make_context/destroy_context.
- .ttf files support for ImGui.
- Some stuff related to windows.
- macro TNT_IMGUI_RUNTIME_CONFIG to enable contents of gui_config.h.

### Changed
- ImGui text color to white by default.
- tnt_imgui_begin() and tnt_imgui_finish() to tnt::ImGui::Begin() and tnt::ImGui::End().
- return type of ImGui widgets from int to bool for using less memory.
- some stuff on gui_config.h.
- The way ImGui handles text.

### Removed
- slider_byte and hslider_byte on ImGui.
- CMakeLists.txt on folder src.

## 2020-04-14
### Added
- `assets` folder containing all engine assets used in main.cpp.
- [.clang-format](./.clang-format).
- [format-proj.ps1](./scripts/format-proj.ps1) to format all .cpp files of the project.
- The *Building the engine from source* section on README.md.
- Added progress_bar widget with it's getters/setters on the ImGui library.
- TNT_ prefix on the include guard of some files that didn't already have it.

### Changed
- Started turning main.cpp into an animation editor.
- Some stuff related to Animations on `class Sprite`.
- The path of the default font on [ImGui.cpp](./src/ImGui/ImGui.cpp#L27).
- The indentation of data inside namespaces.
- id data on every ImGui widget from int to long long.

### Removed
- Coveralls stuff on build.yml.
- Temporarily removed Tiled-related stuff on AssetManager.


## 2020-04-12
### Added
- `#include "tiled/TileMap.hpp` to make `tmx::TileMap` a complete type.
- Full project support for CMake.

### Changed
- Some stuff on CMakeLists.txt.
- AddText on class AssetManager is temporary unavailable.

## 2020-04-11
### Changed
- class RuntimeManager now uses SDL2, even thought it is not completely ready for "release" yet.
- Some float comparisons on Easings.hpp, Rectangle.hpp and Vector.hpp.
- `operator delete` to `operator delete[]` for pixels at [Noise.cpp](src/pcg/Noise.cpp)


## 2020-04-09
### Added
- Added folder rpp on tests.
- Some data and functions on AnimationComponent.
- Minimum value support for slider and hslider widgets.

### Changed
- Moved the Github Actions status icon on to of the README.md file.
- ImGui widgets that modify values now take a pointer to the value rather than a reference.
- min and max at slider and hslider widgets to min_ and max_ so that the names don't collide with any stl algorithms.

### Removed
- The build status header on README.md


## 2020-04-08
### Changed
- [`void Timer::start()`](./include/utils/Timer.hpp#L21) and [`void Timer::stop()`](./include/utils/Timer.hpp#L23) implementation.
- [`Timer::deltaTime()`](./include/utils/Timer.hpp#L27) now calls [`Timer::start()`](./include/utils/Timer.hpp#L21) if the Timer instance is stopped.
- [build.yml](./.github/workflows/build.yml).
- Fixed a problem on [ttf2png.py](./scripts/ttf2png.py).
- Fixed CodeFactor review badge on [README.md](./README.md#L26).
- Marked `class` [File](./include/fileIO/File.hpp)'s, `class` [Expected](./include/utils/Expected.hpp) and `class` [actions::Delay](./include/utils/Actions.hpp) constructors as `explicit`.
- Added `return` statement on `struct` [Rectangle](./include/math/Rectangle.hpp#L41).
- Assigned a value to `location` variable at [Matrix.cpp](./src/math/Matrix.cpp#L134).

### Removed
- include/YamlTypes.hpp
- test.json
- `safe_enum` and `curiously_recurring_template` from include/utils/TypeUtils.hpp.
- redundant `virtual` on functions that are marked as `override`.


## 2020-04-07
### Added
- [.editorconfig](./.editorconfig).
- [README.md](./README.md).
- [CHANGELOG.md](./CHANGELOG.md) (this file).
- New macro [IMGUI_ID](./include/imgui/ImGui.hpp#L6) on [ImGui.hpp](./include/imgui/ImGui.hpp).
- ImGui fonts support, but by using a tool called [ttf2png.py](./scripts/ttf2png.py), which I wrote for this purpose.
- Some new utilities for [TypeLists](./include/utils/TypeLists.hpp).
- [`SDL_Color Window::getClearColor()`](./include/core/Window.hpp#L61)

### Changed
- class InputManager to `namespace tnt::input` on [InputManager.hpp](./include/core/InputManager.hpp#L14).
- The example provided on [main.cpp](./main.cpp).