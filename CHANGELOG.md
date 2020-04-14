# Changelog
All changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## 2020-04-14
### Added
- `assets` folder containing all engine assets used in main.cpp.
- [.clang-format](./.clang-format).
- [format-proj.ps1](./scripts/format-proj.ps1) to format all .cpp files of the project.
- The *Building the engine from source* section on README.md.
- Started working on a progress_bar widget for the ImGui library.
- TNT_ prefix on the include guard of some files that didn't already have it.

### Changed
- The path of the default font on [ImGui.cpp](./src/ImGui/ImGui.cpp#L27).
- The indentation of data inside namespaces.

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