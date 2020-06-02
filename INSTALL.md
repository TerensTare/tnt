# Getting the engine binaries

The engine is planned to be available in two ways: as source code (and be built from the source code), or by getting the pre-built binaries from the Github repo's releases.

## Option 1: Building the engine from source

### Prerequisites (aka getting the dependencies)

The project uses [vcpkg](https://github.com/microsoft/vcpkg) to handle the dependencies as vcpkg is cross-platform, and CMake to build the code, so before you go to the next section, make sure you have installed vcpkg and [CMake](https://cmake.org) (at least version 3.12.4) and set the `VCPKG_DEFAULT_TRIPLET` environmental variable.

*Note*: If you want to build for x86, set `VCPKG_DEFAULT_TRIPLET` as `x86-{yourTargetOS}`. Otherwise, define it as `x64-{yourTargetOS}`.

 After setting up vcpkg properly, please run:
`vcpkg install sdl2 sdl2-image sdl2-mixer sdl2-ttf fmt`.
to get the minimal dependencies needed to build `main.cpp`.

*Note*: If you want to use some specific types asset files, you have to install additional "patches" for sdl2 packages from vcpkg as listed here:

| File extension | Needed package |
|:--------------:|:--------------:|
|.jpg| sdl2-image[libjpeg-turbo] |
|.webp| sdl2-image[libwebp] |
|.tiff, .tif| sdl2-image[tiff] |
|.flac| sdl2-mixer[libflac] |
|.mod| sdl2-mixer[libmodplug] |
|.ogg, .ogv, .oga, .ogx, .ogm, .spx| sdl2-mixer[libvorbis] |
|.opus| sdl2-mixer[opusfile] |
|.mp3| sdl2-mixer[mpg123] |

### Running the build commands.

At the time of speaking, the engine could be used by including header files and adding source files(.cpp) to your compile command. But there is planned support for building the engine as Windows DLL's and C++20 modules. Until then, the build command is as follows:
``` cmd
mkdir build
cd build
cmake ..
```

*Note*: If `cmake ..` fails, please run `cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake`.

*Note*: If you are using Microsoft's Visual C++ compiler with CMake (ex. on Visual Studio), you should run the following commands. Otherwise, skip this step.

``` cmd
:: for x86 build
vsdevcmd.bat

:: for x86-debug build
msbuild TnT.sln
:: for x86-release build
msbuild TnT.sln /p:Configuration=Release


:: for x64 build
vcvars64.bat

:: for x64-debug build
msbuild.exe TnT.sln /p:Platform=x64
:: for x64-release build
msbuild.exe TnT.sln /p:Platform=x64 /p:Configuration=Release
```

### Building the documentation

The documentation is built by default when building with CMake. To build the documentation, you need to have doxygen installed, which can be obtained [here](http://www.doxygen.nl/manual/install.html). To avoid building the documentation, please run `cmake -DBUILD_DOCS=OFF ..` instead of just `cmake ..`.

## Option 2: getting the pre-built binaries.

WIP