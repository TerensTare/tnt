# Changelog

All changes to this project will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## 2020-04-21
### Added
- Widget `checkbox` for ImGui.

### Changed
- `tnt::Timer::deltaTime` to return `std::chrono::milliseconds` by default.

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
- vcpkg.exe to just vcpkg on `CMakeLists.txt` for cross-platform building.

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
- class InputManager to namespace tnt::input on [InputManager.hpp](./include/core/InputManager.hpp#L14).
- The example provided on [main.cpp](./main.cpp).