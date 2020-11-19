# Getting the engine binaries

The engine is planned to be available in two ways: as source code (and be built from the source code), or by getting the pre-built binaries from the Github repo's releases.

## Option 1: Building the engine from source

### Prerequisites (aka getting the dependencies)

The project uses [vcpkg](https://github.com/microsoft/vcpkg) to handle the dependencies as vcpkg is cross-platform, and CMake to build the code, so before you go to the next section, make sure you have installed vcpkg and [CMake](https://cmake.org) (at least version 3.12.4)

- Install vcpkg from your terminal using `git clone https://github.com/microsoft/vcpkg.git`.
- Define the environment variable `VCPKG_ROOT` as the directory where you ran `git clone`, but with `/vcpkg` suffix (`\vcpkg` if you are on Windows).
- Define the environment variable `VCPKG_FEATURE_FLAGS` with value `manifests`.
- Get CMake from [https://cmake.org/download/](https://cmake.org/download/).
- Set environment variable `VCPKG_DEFAULT_TRIPLET` to your corresponding cpu architecture/os. You can skip this step and let CMake choose the triplet that best suits your os/architecture. However, it is recommended that you set this environment variable by yourself.
- Install the dependencies using
``` bat
vcpkg install sdl2 sdl2-image[libjpeg-turbo,libwebp,tiff] sdl2-mixer[dynamic-load,libflac,libmodplug,libvorbis,nativemidi,opusfile] sdl2-ttf sdl2-net sol2 nlohmann-json fmt yasm
```
If you want to build the editor (not only the dynamic library), you should also install `nativefiledialog`.

### Running the build commands.

As of version `0.1.0a12`, the engine is built as a DLL, and the command is as follows:
``` cmd
mkdir build
cd build
cmake ..
cmake --build .
```
The engine's `CMakeLists.txt` also ships with a target that builds the visual editor of the engine. This `option` is `ON` by default. To disable it, please add `-DBUILD_EDITOR=OFF` to the first `cmake` command.

### Building the documentation

The documentation is built by default when building with CMake. To build the documentation, you need to have doxygen installed, which can be obtained [here](http://www.doxygen.nl/manual/install.html). To avoid building the documentation, please run `cmake -DBUILD_DOCS=OFF ..` instead of just `cmake ..`.

## Option 2: getting the pre-built binaries.

WIP