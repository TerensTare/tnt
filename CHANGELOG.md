# Changelog

All changes to this project will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [0.1.0a19]
### Added
- `tnt::unroll` on `types/TypeUtils.hpp`, which emulates a partially unrolled loop.
- `json/JsonEnum.hpp`, which adds support for automatically generating json bindings for desired enum types on request. For enabling automatic enum bindings, please modify `tnt::enum_traits<T>` from `types/EnumTraits.hpp` to `true` for your enum `T` like this:
```cpp
    enum my_enum { /* some custom values */ };

    #include "types/EnumTraits.hpp" // needed for tnt::enum_traits

    template <>
    struct tnt::enum_traits<my_enum>
    {
        inline static constexpr my_enum count = my_enum::count; // change this if your enum doesn't have count at the end as a "sentinel"
        inline static constexpr bool jsonable = true;
    };
```
- `tnt::enum_sentinel<T>`, a variable template that signalises "end of parsing" to enum-stringifying functions on `mirror/Light.hpp`. It can be specialised for your custom enums, and it defaults to `T::count`.
- `tnt::enum_name` for getting a string representation of an enum's name either at compile time or run time.
- `tnt::enum_value` for getting an concrete enum value from a string representation either at compile time or run time.
- Support for `tnt::task<void>`.
- `mirror/Light.hpp` which contains some basic reflection facilities. `tnt::type_name` and `tnt::value_name` now live there.
- `tnt::conceptify` on `types/TypeUtils.hpp`, which helps convert simple type traits to concepts without you needing to write `requires`.

### Changed
- Reduced template instantiations on the new type lists implementation. You can find them on `types/TypeLists.hpp`.
- Improved `tnt::range` to cover some corner cases.
- `tnt::enums` now returns an array of pairs containing the name and the value of the enum.
- `tnt::Vector` is now trivially default constructible. Also the constructor which has a single parameter is now explicit.
- `include/json/*`'s folder files now have a corresponding source file on `src/json`. Also the "registration" technique is changed, but shouldn't affect the code that uses these files.
- `tnt::doo::system_base<T>::clear` now works even if `T` doesn't provide a `clear` method by calling `remove` on each object on `T::active`. Also `tnt::doo::system` doesn't `require` types to provide a `clear` method anymore.
- Methods of `tnt::system<T>` are now conditionally `noexcept` depending on the implementation of these methods on `T`.

### Deprecated
- `tnt::Expected`. Use `tnt::expected` from the same file instead as it offers exceptionless error handling and `constexpr` usage.

### Removed
- Old typelists implementation. It is replaced by the implementation that was previously found on folder `exp`.
- `[[deprecated]]` functions on `tnt::doo::physics_sys`.


## [0.1.0a18]
### Added
- `operator==` for `tnt::bit_mask` types.
- `utils/SDL_forward.hpp`, which forward-declares some opaque SDL `struct`s.
- `tnt::Vector` now is move assignable.
- `tnt::value_name` which returns a string representation of the value passed as a template parameter. It can be especially useful when used with enums.
- Conversion operator from `tnt::basic_hashed_string<Char>` to `std::basic_string_view<Char>`.

### Fixed
- Some warnings related to the naming of some local variables.

### Changed
- Operators overloaded for `tnt::bit_mask` are now marked `noexcept`.
- Separated the documentation part of the build from the rest. See `cmake/Doxygen.cmake`.
- `pcg/Random.hpp` now has a corresponding source file.
- Rewrote `tnt::generator<T>` to allow custom allocator support.
- Minor quality of life improvements for `tnt::generator<T>` and `tnt::task<T>`.


## [0.1.0a17]
### Added
- `{fmt}` support for `tnt::type_info`. See `format/FormatedTypeInfo.hpp`.
- Support for the function from `core/Graphics` on Lua (table `tnt:gfx`). There is a new entry on `tnt::lua::lib` named `graphics` and some entries now have an updated value.
- `tnt::hashed_string`, a non-mutable string with a pre-computed hash value on `types/HashedString.hpp`. `std::hash` (`types/HashedString.hpp`) and `fmt::formatter` (`format/FormatedHashString.hpp`) specialisations are provided.

### Fixed
- `tnt::type_info::name()` will now correctly show the type name stored.

### Changed
- `tnt::type_info::operator==` is now `[[nodiscard]]` to avoid accidentally using comparing instead of assignment.
- `tnt::type_info::name()` will now return a `std::string_view`.
- `tnt::type_info` is now `final` to avoid unintended usage.
- `tnt::type_id` will not strip `const&` from the type name anymore.
- Fixed warnings about `tnt::doo::physics_sys::addForce` being used on `tnt::doo::steering_sys`.
- `tnt::AudioPlayer` now contains a pointer to the audio cache and one to the sfx cache.
- `tnt::Snipper`, `tnt::vfs_handle` and `tnt::vfs::*` now use `std:unordered_map`.
- `tnt::Snipper`, `tnt::vfs_handle`, `tnt::vfs::*`, `tnt::asset_cache` pre-defined specialisations now use `tnt::hashed_string::hash_type` as key.
- Updated `extra/.editorconfig`to match the style used on the codebase.

### Removed
- `physics` folder, it was unused.


## [0.1.0a16]
### Added
- `tnt::type_name` on `TypeInfo.hpp` if you need only the name of a type.
`tnt::operator<<` for `tnt::type_info` so that it can be used with `std::cout`.
- `tnt::Vector::operator*` with `float` as first argument, making some code easier to read.
- `tnt::physics_sys::force`, that stores the force being applied to the objects on a certain moment. Related functions are updated accordingly.
- `tnt::is_debug_v` and `tnt::is_release_v` on `utils/Traits.hpp`.
- `extra/tnt.natvis` will be installed when building the `install` target.
- `utils/FLogger.hpp`, which adds support for writing log output to a file by using functions on `tnt::logger`.
- `tnt::lua::registerLoader` to add the TnT modules loader to the given lua state.
- `tnt::drawCircle` and `tnt::fillCircle` on `core/Graphics`.
- Deduction guides for classes on `types/ScopeUtils.hpp`.

### Changed
- Updated `tnt::type_info` and `tnt::type_id<T>` to work without `friend`.
- `tnt::physics_sys::AddForce` and `tnt::physics_sys::addGlobalForce` are no longer profiled.
- `tnt::ImGui::WindowFlags` is now 8 bit.
- `tnt::logger` will now print the logging message type from `ERROR:` to `[ERROR]` to provide more readable output.
- Moved `tnt::logger::printable` to `namespace detail`.
- TnT libraries on Lua now MUST be `required` when used with `tnt::scripts_sys` from the Data Oriented ECS. Otherwise, the good ol' `lua::load*` functions are still available for use.
- The third argument of `tnt::scripts_sys::add_object`, since TnT libraries can now be `require`d from a Lua script.
- `tnt::crtp<T>::super` is now `protected`.
- Functions on `core/Graphics` are now marked as `noexcept`.

### Fixed
- Small tweaks.
- Some incorrect things on `exp/TypeListsv2.hpp`.
- Some incorrect things on `doo_ecs/System.hpp`.

### Deprecated
- `tnt::physics_sys::addForce` and `tnt::physics_sys::addGlobalForce`. Instead, just add the desired force to the `tnt::physics_sys::force[id]` and `tnt::physics_sys::totalForce` variables accordingly.

### Removed
- `tnt::physics_sys::resolveVelocity` and `tnt::physics_sys::resolveInterpenetration`. Use `tnt::physics_sys::resolve` instead.
- `tnt::crtp<T>::super`. It gave issues when assigning a derived class. Use `tnt::crtp<T>::base` instead.


## [0.1.0a15]
### Added
- `tnt::y_comb` on `types/TypeUtils.hpp`.
- `tnt::vfs_handle` if you want to have separate virtual file systems. It is thread-safe, like the functions on `tnt::vfs` will be soon.
- `tnt::crtp<T>::super`, which is the same as calling `tnt::crtp<T>::base()`.
- `doo_ecs/System.hpp`, which contains some utilities related to Data Oriented ECS systems.
- `utils/Bench.hpp` which will replace `utils/Benchmark.hpp`.
- `tnt::sized_any<S>::operator bool` to check the validity of the content of the any type.

### Changed
- `tnt::crtp<T>` is now default constructible and  `constexpr`-ready.
- `TypeUtils.hpp` is now moved on folder `types`.
- `PROFILE_FUNCTION` now works with the three major compilers.
- `tnt::any` is now `tnt::sized_any<S>` where S denotes the size of the storage for the any. It is deduced by default or can be set by the user. If you want to use different any objects with different sizes on an array/vector, use `tnt::any` which is equal to `tnt::sized_any<(std::size_t)-1>` and has a default size.

## Deprecated
- `utils/Benchmark.hpp`, but you should still use it, until `utils/Bench.hpp` is finished.


## [0.1.0a14]
### Added
- Folder `types`, which contains type-related utility files.
- `mirror/TypeInfo.hpp` and `mirror/Any.hpp`.
- `exp/TypeListsv2.hpp` which will replace the old implementation in the future.
- `extra/tnt.natvis` which lets you check the values of `tnt::` data types on the Visual Studio debugger. NOTE: Data Oriented Systems are not included.
- Destructor `tnt::Expected<T>` which correctly cleans memory occupied by the object.
- `tnt::scripts_sys::reload`.
- `to_json` method to Data Oriented ECS systems.
- A window icon for the TnTEditor.
- New overload of `tnt::Window::setIcon`, which loads an icon for the window from a given path.
- Support for loading only desired libraries of TnT for each object part of `tnt::doo::scripts_sys`.
- Support for logical operators for `tnt::lua::lib`.
- More specific flags to `tnt::lua::lib`.

### Fixed
- Minor things on the TnT install system from `CMakeLists.txt`.
- Rewrote a part of `README.md` to be more informative.
- Error with auto-generated `tnt::operator |=`, `tnt::operator &=` and `tnt::operator ^=` for bitmask types.
- `tnt::has_flag` from `utils/BitFlags.hpp` now supports `const` flags.

### Changed
- `tnt::Snipper::onModify` now returns `void`.
- The TnT editor will now be installed too when the `install` CMake target is selected.
- Data Oriented Systems now have no constructor/destructor.
- `tnt::lua::loadSparseSet` will now load `tnt::sparse_set<tnt::doo::camera>` along with `tnt::sparse_set<tnt::doo::object>`.
- The json structure of the scripting component is now:
```json
    {
        "script": {
            "file": "script.lua",
            // optional, core by default.
            "libs": ["core", "math"]
        }
    }
```
- Type of second argument of `tnt::lua::load` from `std::span<const tnt::lua::lib>` to `tnt::lua::lib`.

### Removed
- `CONTRIBUTING.md`. Everybody is now free to ask for support whereever it seems reasonable.
- `core/Context.hpp`.


## [0.1.0a13]
### Added
- `input:mouse_pos` to the Lua API.
- Small improvements to the editor UI.
- `tnt::doo::null_v`, which is generic version of `tnt::doo::null`. NOTE: It only works with types that satisfy `concept std::unsigned_integral`.
- Example project showing how to use skeletal animation with TnT.
- `remove` method from `tnt::asset_cache`.
- `async/Generator.hpp` and `async/Task.hpp`.
- `core/Graphics` which provides some helper functions related to drawing lines. More functions to come.
- `tnt::Window::setWidth`, `tnt::Window::setHeight` and `tnt::Window::setSize`, to manipulate the size of the window.
- New example named `project`, showing a simple project that can be loaded on the editor.
- `remove` member function for the systems of the Data Oriented ECS that lets you remove entities from the given system and `clear` which removes all the entities from the given system.
- `utils/PolyResource.hpp`, which contains a helper to handle data that uses `std::pmr::polymorphic_allocator`.
- `tnt::template_traits`, which give you the tools to play around with templates.

### Fixed
- Some "incomplete" things in `utils/Sort.hpp`.
- Lua functions of Data Oriented ECS systems failing unexpectedly.

### Changed
- `tnt::vfs::absolute` now uses `<filesystem>` instead of `<SDL2/SDL_filesystem.h>`.
- `tnt::doo::camera` is now `std::uint32_t`. This is done to save space.
- Moved `utils/CoUtils.hpp` to folder `async`.
- Added some small features to the editor.
- TnT now builds with fast floating point model.
- The systems of the Data Oriented ECS will now check for inexistent objects on `add_object`.
- Rewrote `utils/TypeLists.hpp` to use lowercase for the name of the `struct`s and support for more than 2 types stored on a `type_list`.

### Removed
- `tnt::sparse_set<T>::edit`.


## [0.1.0a12]
### Added
- Support for steering behaviours on Lua.
- Deduction guides for `tnt::sparse_iterator<T>`.
- `utils/LuaUtils.hpp`, which will contain functions that export some stuff from folder `utils` to lua. For now, only `tnt::sparse_set<T>` is supported.
- Some `noexcept` on `tnt::sparse_set<T>`.
- You can now assign a `tnt::sparse_sentinel` to a `tnt::sparse_iterator`.
- `tnt::doo::sprites_sys::target_cam`, to set a camera as a target for the renderer.
- Some small missing parts of the documentations.
- Section on how to build the engine as a DLL on `INSTALL.md`.
- `tnt::sparse_set<T>::edit`, `tnt::sparse_set<T>::index`,`tnt::sparse_set<T>::insert`, `tnt::sparse_set<U>::operator==`.

### Fixed
- A small bug with comparing `tnt::sparse_iterator<T>` and `tnt::sparse_sentinel`.
- Exported `tnt::lua::load`.
- `exe` linked with `TnT.dll` crash randomly. NOTE that if you need to use the Data Oriented ECS (you normally need) and the Lua bindings for the Data Oriented ECS (`src/tolua/LuaDooEcs.cpp`), you have to build the source files on the `src/doo_ecs` folder together with your executable.

### Changed
- `tnt::lua::load` now needs `std::span<const tnt::lua::lib>` as an argument.
- `tnt::sparse_iterator`'s `explicit` ctor now accepts a `tnt::sparse_set<T> const&` instead of `tnt::sparse_set<T> const*`.
- `tnt::sparse_set<T>` is now copy-constructible.
- Renamed all the member of the systems of the Data Oriented ECS that holded the id-s of the objects to `active`. Also changed their data type from `std::vector<object>` to `tnt::sparse_set<object>`.

### Removed
- `tnt::lua::lib::doo_ecs`. Load the Data Oriented ECS by hand (just call `tnt::lua::loadDooEcs`).
- `tnt::doo::has_object` and `tnt::doo::json_has`. Use `tnt::contains()` from `utils/Containers.hpp` instead.


## [0.1.0a11]
### Added
- `include/TnT.hpp`, which `#include`s all the current headers of the TnT Engine SDK.
- `utils/SparseSet.hpp`, which contains an implementation of a sparse set.
- Support for building the engine as a dynamic library.
- `tnt::doo::scripts_sys::Init()` and `tnt::doo::scripts_sys::call()`.
- More `static` on some global variables on `.cpp` files.
- `utils/Benchmark.cpp`.

### Fixed
- Lua bindings are working!!

### Changed
- `tnt::Timer` now uses `std::chrono::high_resolution_clock`.
- Rewrote `utils/Benchmark.hpp`.

### Removed
- `utils/SDL_Utils.hpp`.
- `fileIO/AssetManager`.
- `cmake/Setup.cmake`.
- `include/tiled` folder.


## [0.1.0a10]
### Added
- `static` on some global variables on `.cpp` files.
- `tnt::doo::cameras_sys::zoom_to_fit`.
- `tnt::halton1` and `tnt::halton2` on `pcg/Random.hpp`, which generate a random number base on Halton sequences.
- `tnt::doo::scripts_sys::call`, which calls a function from the lua script.
- Support for `fileIO/VirtualFS`' alias paths for the built-in `asset_cache`s.

### Changed
- Renamed `concept tnt::doo::basic_system` to `concept tnt::doo::system` and removed some unnecessary requirements. 
- `tnt::doo::animation_comp`'s and `tnt::doo::object_data`'s ctor is now moved to the respective header file.
- Data Oriented Systems now don't need to be non-copyable.
- `tnt::lua::load` now accepts a `sol::state_view` as the first argument.
- `add_object` and `from_json` methods on each Data Oriented System (except `objects_sys`) now take `object const&` as the first parameter.
- `tnt::doo::objects_sys::from_json` now returns `tnt::doo::object`.
- Data Oriented Systems now don't need to implement `add_invalid`.
- `tnt::asset_cache` now needs an `unsigned int` as a template parameter, rather than just an `int`. Also the built-in `asset_cache`s will now use `std::pmr::unordered_map` instead of `std::pmr::map`.
- Rewrote `fileIO/VirtualFS`.

### Removed
- `concept tnt::doo::system`.
- `game.lua`.
- `tnt::lua_ctx` from `utils/LuaLoader.hpp`.
- `add_invalid` from all built-in Data Oriented System.
- `core/Camera`, `core/Space`, `core/Scene`, `ecs` and `ecs2` folders.


## [0.1.0a9]
### Added
- `tnt::randomUnitVector` on `pcg/Random.hpp`.
- `tnt::doo::null` on `doo_ecs/Base`, which serves as an id for non-existent entities on a system.
- `tnt::doo::physics_sys::resolveVel` and `tnt::doo::physics_sys::resolveInterpenetration`, which are called by `tnt::doo::physics_sys::resolve`.
- `tnt::has_flag`, `tnt::set_flag`, `tnt::unset_flag` on `utils/BitFlags.hpp`.
- `tnt::Rectangle::TopLeft`, `tnt::Rectangle::TopRight`, `tnt::Rectangle::BottomLeft` and `tnt::Rectangle::BottomRight`, which return the corresponding corner of the rectangle.
- `concept basic_system` on [doo_ecs/Base.hpp](./include/doo_ecs/Base.hpp), which contains all the members that a system must have.
- Support for drawing objects on a camera/multiple cameras for the Data Oriented ECS. See [doo_ecs/Cameras.hpp](./include/doo_ecs/Cameras.hpp).

### Changed
- Optimized `tnt::RotateVector` for cases when the angle is 0.
- The following files are deprecated, and will be removed on the next version update:
`core/Camera`, `core/Space`, `core/Scene`, all the files on `ecs`, `ecs2` and `tiled`. Use files on `doo_ecs` as a replacement instead.
- `tnt::doo::physics_sys::resolve` now resolves both the velocity and the interpenetration of the colliding objects.
- Updated `examples/doo_ecs_ex.cpp` to include the new cameras system.
- `tnt::doo::sprites_sys::Draw`, now needs the id of a camera as the last argument, set it to `-1` (the default) to switch to old-style drawing.
- Moved `object_data`, `concept system` and `objects_sys` to [doo_ecs/Objects.hpp](./include/doo_ecs/Objects.hpp).

### Removed
- `utils/LuaManager`. Use files on the `tolua` folder instead.
- `tnt::Window::Draw`. Use `tnt::doo::sprites_sys::Draw` instead.


## [0.1.0a8]
### Added
- `struct`s for checking supported SIMD instructions by the machine on `utils/Traits.hpp`.
- `vcpkg.json`, which will automatically install the dependencies of the engine using vcpkg.
- Support for more steering behaviours.
- `concept tnt::doo::system`, which contains the basic requirements that a data type should have to be called a system of the Data Oriented ECS.
- `concept tnt::logger::printable`, which checks if a type can be used as a parameter to the engine's logger.
- `tnt::doo::sprites_sys::draw_area`, which returns the area where the object passed as a parameter will be drawed.

### Fixed
- An issue with `fileIO/TextureAtlas`.
- Some issues related to non-correct calculations with global data on the Data Oriented ECS.

### Changed
- Updated Github Actions, Microsoft Azure Pipelines and AppVeyor build due to changes to `CMakeLists.txt`.
- Updated `INSTALL.md` to explain how the build steps change using vcpkg manifests.
- CMake will now automatically find vcpkg, but you have to define the `VCPKG_ROOT` environment variable.
- All the functions of `tnt::logger` now require that their parameters satisfy `tnt::logger::printable`.
- All the Data Oriented ECS systems will now check if an object is part of the system when calling `Update`. `tnt::doo::sprites_sys` will do the same thing on the `Draw` function.
- Rewrote `tnt::Rectangle::Outside` using `tnt::Rectangle::Contains`.
- Rewrote `utils/Logger.hpp` to have no overload (one function for each logging level).
- Some style-related stuff on `Doxyfile.in`.
- Modified the example program to show how to use the global coordinates with the Data Oriented ECS.

### Removed
- Folder `ai`, which contained `ai/steer/Steering` files.
- `tnt_imgui_close`. ImGui-related resources will be freed automatically on exit.


## [0.1.0a7]
### Added
- `doo_ecs/Steering`, which adds support for steering behaviours to the objects that are part of the physics system.
- `tnt::AngleOfR`, which is just the same as `tnt::AngleOf`, but the result is in radians.
- Overload of `tnt::AngleOf`, which returns the rotation of a `tnt::Vector`.
- Support for giving physics objects a maximum velocity and acceleration. If one of these properties is specified when loading from a json chunk, the other is mandatory to be specified.
- `tnt::doo::has_object` on `doo_ecs/Base`, to check whenever a certain DOO ECS system have a member with the given id.
- An experimental work-in-progress implementation for handling skeletal animations (`doo_ecs/Bones`).
- `CODESTYLE.md`, which contains a guide to the coding style used on the engine's source code.
- A partial experimental implementation of actions and action lists, which supports coroutine functions.
- Support for logging with colored output.
- `tnt::doo::object_data::parent` and `tnt::doo::objects_sys::parent`, which keep track of the parent of the object. That means the Data Oriented ECS objects now support global/local coordinates. Use `-1` to show that the object has no parent. The members `angle`, `scale` and `pos` now store the local coordinates. The same applies to physics' `vel`, `maxVel`, `accel` and `maxAccel` properties. NOTE: when constructing a new object for `objects_sys` or `physics_sys`, the upmentioned parameter members are treated like using the global context.
- `gAngle`, `gScale`, and `gPos` on `tnt::doo::objects_sys`,
which get the required data related to the global context.
- `tnt::doo::objects_sys::set_parent`, which changes the parent of the given object.

### Changed
- The Data Oriented ECS now updates and draws objects using global coordinates.
- `tnt::bubble_sort_fn` is now `final`.
- `tnt::doo::object_data` is now `final`.
- Rewrote some of the implementation of `utils/TypeLists.hpp`.


## [0.1.0a6]
### Added
- `Contributors` section on `README.md`.
- `add_invalid` on each system of the Data Oriented ECS except `objects_sys`.
- Partial support for Joysticks on `core/Input`. Joysticks are automatically detected when connected/removed from the device.
- `utils/CoUtils.hpp`, which contains some coroutine-related utilities.
- `cmake/Setup.cmake`, which contains some functions that wrap a part of the code on `CMakeLists.txt`.


### Fixed
- Incorrect behaviour of `tnt::Dot` on `math/Vector.hpp`. (thx [cgyurgyik](https://github.com/TerensTare/tnt/pull/8)).


### Changed
- `tnt::logger::error` is now marked as `[[noreturn]]`.


### Removed
- Redundant `inline` on `constexpr` functions. (thx again cgyurgyik).


## [0.1.0a5]
### Added
- `tnt::doo::scripts_sys`, which connects `.lua` scripts to objects of the Data Oriented ECS.
- `tnt::bubble_sort_view`, `tnt::bubble_sort_fn` and `tnt::views::bubble_sort` on `utils/Sort.hpp` which wrap the bubble sort algorithm.

### Changed
- Marked some stuff as deprecated. A warning will be emited when you use them. The affected files/classes are: `tnt::AssetManager`, `tnt::VirtualFS` and `LuaManager.hpp`/`LuaManager.cpp`.
- Trailing `0` after comma won't show on ImGui's `silder_*` and `hslider_*` functtions.
- Rewrote a part of `exp/Runtimer`.
- `tnt::doo:sprites_sys::add_object` now accepts `tnt::doo::sprite_comp const&`.
- `tnt::doo::objects_sys::get_data` is now `[[nodiscard]]`.
- Systems from the Data Oriented ECS now are non-copyable and accept `tnt::doo::object const&` instead of just `tnt::doo::object` as function parameters.
- The implementation of the functions of the Data Oriented ECS is now located on `.cpp` files.

### Removed
- An overload of `tnt::doo::sprites_sys::add_object`.


## [0.1.0a4]
### Added
- `tolua` folder, which contains separated files for Lua bindings. The files `utils/LuaManager.hpp`/`utils/LuaManager.cpp` are now deprecated, and you are encouraged to use the headers located on `tolua` folder.
- Support for non-movable bodies on `physics_sys`.
- Support for detecting and resolving collisions between `physics_sys` objects. See `tnt::doo::physics_sys::colliding` and `tnt::doo::physics_sys::resolve`.
- Support for adding/removing global forces applied to the objects of `tnt::doo::physics_sys`.
- `inline namespace phys`, which provides some physics-related constants, ex. forces that can be applied to objects (like gravity).
- More `constexpr` for `tnt::Vector`.
- Use three way comparison for `tnt::Vector` if supported by the compiler.
- `utils/BitFlags.hpp`, which helps on generating bit manipulating operators for the enums the user selects.
- `utils/Traits.hpp` which provides some type traits utilities.

### Changed
- Moved `tnt::is_detected`/`tnt::is_detected_v` to `utils/Traits.hpp`.
- `tnt::doo::physics_sys::add_object` now accepts a `physics_comp const&` as an argument.
- The Data Oriented ECS systems now reserve memory for 10 objects instead of 5 when needed.
- The Data Oriented ECS systems now check if objects have a specific component when loaded from a .json file.

### Removed
- `utils/LuaManager.hpp`/`utils/LuaManager.cpp`. Use files located on the `tolua` folder instead.
- `utils/LifetimeManager.hpp`.
- `utils/Observer.hpp`.
- `tnt::doo::phys_comp`.
- `tnt::scoped_thread`.


## [0.1.0a3]
### Added
- `tnt::input::Update`, which calls `tnt::input::updatePrevious` and `tnt::input::updateCurrent`.
- `tnt::render_lock`, a RAII class which clears the given window on its ctor, and calls `Render` on its destructor.
- `constexpr` on some members of `tnt::Vector`, `tnt::Rectangle`, and some other math functions.

### Changed
- Implementation of easings to use lambdas and `constexpr`.
- All `tnt::default_*_cache` (except of `tnt::default_texture_cache`) now return a `small_cache` (initial size of 10) instead of a `medium_cache` (initial size of 50).
- Made some performance improvements for `tnt::AudioPlayer`.
- Rewrote a part of ImGui's backend to reduce dynamic memory allocations.
- Implementation of `tnt::ensure`, `tnt::safe_ensure` and `tnt::check`. No behaviour change involved.

### Removed
- `json/JsonDooObject.hpp`.
- `tnt::run_after` on `utils/TypeUtils.hpp`.
- `tnt::ImGui::destroy_context`. Context resources are now freed automatically.
- `tnt::ImGui::update_context` because `tnt::ImGui::End` now calls it for the user.
- `tnt::doo::objects_sys::Update` and `tnt::doo::objects_sys::Draw`. Use `tnt::doo::physics_sys::Update` and `tnt::doo::sprites_sys::Draw` instead.
- `concept tnt::doo::system`.


## [0.1.0a2]
### Added
- Missing documentation over some places.
- `tnt::if_then` and `tnt::if_else` on `TypeUtils.hpp` to avoid conditional branching.
- `tnt::TextureAtlas` asset type and specializations for `tnt::asset_cache` to support it. Use a texture atlas when you have all your textures arranged on a single image file to gain performance.
- `tnt::Timer::deltaCount`, which is a shorthand to `tnt::Timer::deltaTime::count()`.
- Animations support for the new data oriented ecs.

### Changed
- `tnt::Timer::deltaTime` now calls `reset`. Also it returns the duration with `float` base. That means the `Update` functions now need `float` instead of `long long`.
- tnt::Timer is now header only.
- `tnt::overload` is now `final`.
- `tnt::asset_cache<T, I>::load` is now `public`.
- `tnt::Rectangle` conversion operators now are not `explicit`.
- Rewrote a part of the backend of `core/Input`. The user no longer needs to call `tnt::input::close`. It is now removed.

### Removed
- Visitor-related stuff and `tnt::is_convertible` on `utils/TypeUtils.hpp`.


## [0.1.0a1] - 2020-07-13
### Added
- An experimental reflection system. See `utils/Mirror.hpp`.
- Header `utils/Assert.hpp`.
- `tnt::overload` on `utils/TypeUtils.hpp`.
- A new Data Oriented Entity Component System.
- `tnt::asset_cache` to replace `tnt::AssetManager`. Note that `tnt::AssetManager` will be deprecated and then removed on a future release.
- Load a game script from cmd.
- Hot code reloading for `.lua` files.
- Folder `format` and `json`, which contain code for serializing the engine types to `{fmt}` and `nlohmann::json`.

### Changed
- `tnt::Rectangle` is now DefaultConstructible`.
- Moved `STATIC_CHECK` from `Utils.hpp` to `utils/Assert.hpp`. Moved container-related stuff from `Utils.hpp` to `utils/Containers.hpp` and `utils/Convert.hpp`.
- `tnt::AssetManager` is now `deprecated`.

### Removed
- `tnt::Synchronized` from `utils/TypeUtils.hpp`.
- `Utils.hpp`, `FormatedTypes.hpp` and `JsonTypes.hpp`.


## 2020-06-20
### Added
- Link to Gitter room on `CONTRIBUTING.md`.
- `assets/particle.png`.

### Changed
- Most of the content of `ecs/Particle`.


## 2020-06-19
### Added
- Link to the official subreddit on `CONTRIBUTING.md`.

### Changed
- Lua functions on `main.cpp` should be now on table `tnt`. ex. `function tnt.draw`.
- `tnt.do_imgui` is now optional.

### Removed
- `fileIO/File.hpp`/`fileIO/File.cpp`.

## 2020-06-18
### Changed
- `tnt::file::Snipper` is now `tnt::Snipper`, and is a `final` class.
- `tnt::Snipper::isModified` is now `public`.

### Removed
- Some unnecessary lines on `.gitignore`.


## 2020-06-16
### Changed
- `tnt::Window`s created with the second constructor now have `SDL_WINDOW_SHOWN` flag by default.


## 2020-06-15
### Added
- `tnt::lua::loadSpriteComp`/tnt::lua::loadPhysComp`.


## 2020-06-14
### Added
- `tnt::lua::loadSpace`/`tnt::lua::loadScene`.

### Changed
- `tnt::Space::addObject` now doesn't use concepts, but needs an `tnt::Object const*` as a second argument. This decision is made for performance reasons.


## 2020-06-13
### Added
- `assets/TnT.png` and `assets/TnT.ico`.
- Some stuff on `CMakeLists.txt` to build an installer for the engine.
- `tnt::lua::load` to load all needed packages. It works similar to `sol::state_view::open_libraries`.
- `tnt::lua::lib` to be used as argument of `tnt::lua::load`.


### Fixed
- Some stuff on `README.md`.


### Changed
- `Player::to_lua` on `main.cpp` is now `static`.
- `tnt::lua::*` functions now return `sol::table`.


### Removed
- `math/GenericMath.hpp`, but its contents are available on `math/MathUtils.hpp`.
- `include/version.h.in`.


## 2020-06-12
### Added
- `ai/steer/Steering.hpp`/`ai/steer/Steering.cpp`. It contains some basic steering behaviours that can be applied to `tnt::Object`s that have a `tnt::PhysicsComponent`.


### Fixed
- Steering functions not producing desired behaviour.


## 2020-06-11
### Added
- `tnt::Vector::operator *=`/`tnt::Vector::operator /=` overload that accepts a `float` as right-hand-side.
- `struct tnt::is_detected` on `TypeUtils.hpp`.
- `tnt::PhysicsComponent::getMaxVelocity` and `tnt::PhysicsComponent::getMaxAcceleration`.
- `tnt::PhysicsComponent::getDirection` which returns `velocity.Normalized()`.
- `tnt::PhysicsComponent::getSpeed` which returns `velocity.Magnitude()` and `tnt::PhysicsComponent::getMaxSpeed` which returns `maxVelocity.Magnitude()`.
- `tnt::AngleOf` which returns the angle formed by `tnt::Vector`s passed as a parameter.


### Changed
- `tnt::Dot` now returns `lhs.x*rhs.x+lhs.y+rhs.y`. Use `tnt::AngleOf` to get the angle formed by the parameter `tnt::Vector`s.


### Removed
- `fileIO/Reader.hpp` and `fileIO/Write.hpp`.


## 2020-06-05
### Added
- Some utility math functions on `math/GenericMath.hpp`.


## 2020-06-04
### Added
- header `math/GenericMath.hpp`. There you will find `PI` (to be removed when C++20 adds support for some 'special' numbers like pi and e) and templated math-related lambdas like lerp, blerp and beziercurve.
- `tnt::pcg::randomVector`.
- Library's ImGui bindings for Lua.
- `concept tnt::object`. Please use it as a function argument to reduce the number of `new`/`delete` calls.

### Changed
- `tnt::Space::addObject` now accepts a `const` reference of a class derived from `tnt::Object` instead of a `tnt::Object*`.

### Removed
- The overload of `tnt::Space::addObject` which took a `std::pair` as an argument.

## 2020-06-03
### Added
- Support for minimalistic physics with `tnt::PhysicsComponent`. Now `tnt::Space`-s will update an object's physics if it `has<PhysicsComponent>()`.

## 2020-06-02
### Added
- `FormatedTypes.hpp` that contains specializations of `fmt::formatter` for engine types.

### Changed
- Updated `CONTRIBUTING.md`.

## 2020-06-01
### Added
- Worked on the new ECS implementation. You can find it on the `ecs2` folder, but it is not guaranteed to replace the current stable ECS.

## 2020-05-30
### Added
- `tnt::ImGui::BeginSection()` and `tnt::ImGui::EndSection()`.

### Fixed
- Some small "problems" with the look of some ImGui widgets.

### Changed
- Updated the documentation for bulding the engine.
- `tnt::ImGui::Begin()` to use the passed `tnt::WindowFlags` parameter for window customization. Note that only some of the flags are available for the moment.


## 2020-05-29
### Changed
- Restructured ImGui's backend with reduced `new`/`delete` calls to gain some performance.

## 2020-05-28
### Removed
- `ecs/Component`. `tnt::PhysicsComponent` is now located on `ecs/RigidBody`.
- `physics/Box` and `physics/Quadtree`.
- Most of the functions of `imgui/gui_config.hpp`.


## 2020-05-27
### Changed
- The window size of the example in `main.cpp` is now `1280x720`.


## 2020-05-26
### Added
- `tnt::Scene`'s constructor now needs a `std::string_view` which contains the path of the background. If it is equal to "", no background is drawed.
- `tnt::Space::isActive`. `tnt::Space`s are not active if none of their `tnt::Object` is active. If a `tnt::Space` is not active, it is not updated nor rendered.

### Changed
- `tnt::Window::getWidth` and `tnt::Window::getHeight` are now marked as `const`.


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
- Some functions on the implementation of `tnt::Scene`.
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


### Fixed
- A typo related to `SDL2_DIR` on `azure-pipelines.yml`.


### Changed
- Every `class` derived from `class Component` is (and should be) `final`, derived ONLY from `clas Component` and NOT `virtual`-ly inherited.
- Moved the `class Timer` test to folder `test/timer/`.

### Removed
- `#include <SDL2/SDL.h>` on [Component.hpp](./include/ecs/Component.hpp).

## 2020-04-29
### Added
- Conditional check before `delete`-ing an `Object`'s `parent`.
- Ability to move and resize ImGui windows. Widgets won't be drawn if they won't fit on the window.


### Fixed
- Missing `#include` on `AudioPlayer.cpp`.


### Changed
- Renamed `concept camera_type` to `concept camera`.
- Renamed `StackAllocator` to `stack_allocator`. It is still not functional.

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
### Fixed
- A bug with `tnt::ImGui::button()` showing the wrong color.


### Changed
- `tnt::ImGui::menu()` now needs a `std::string_view*` rather than `std::string*` to conserve space.
- Now ImGui widgets require `std::size_t` for their id rather than `long long`, so that the id is not negative and it is compatible with `std::hash`.
- The title on ImGui's windows is now more centered.
- `tnt::ImGui::checkbox()` now doesn't need to have an `id`, `x` and `y`, because it is drawn on the window. Just a `window` where to draw it, the `text` to draw to it and a `bool *value`. The same with `tnt::ImGui::button()` and `tnt::ImGui::menu()`.
- `tnt::ImGui::progress_bar()` now has an offset between it's body and it's filled part and doesn't need `x` and `y` as parameter, but needs a `text` to draw on its side.
- `tnt::ImGui::button()`'s width is now dependent on the size of the text you want to display on it.
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