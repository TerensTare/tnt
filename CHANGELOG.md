# Changelog
All changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).


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