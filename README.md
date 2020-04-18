# The TnT Engine

![build](https://github.com/TerensTare/tnt/workflows/build/badge.svg?branch=master)

TnT is a 2d Game Engine written in C++. At the time of speaking the engine uses
features from the C++17 standard (as there is no C++ compiler that fully
supports C++20 features), but one of the goals of the engine is to use C++20
features wherever possible.

## Project Goals

The project aims to make game development easy enough for anybody that doesn't
know C++. Meanwhile the engine should be powerful enough for a C++ developer.

## Features

- Fast 2d rendering.
- Efficient Entity management.
- Fast and easy GUI library.
- Lightweight logging.
- Networking support.
- Data serialization (save into/load from file).
- Scripting support.

## Code quality

|Code checker|Badge|
|:----------:|:---:|
| CodeFactor | [![CodeFactor](https://www.codefactor.io/repository/github/terenstare/tnt/badge)](https://www.codefactor.io/repository/github/terenstare/tnt) |
| Codacy     | [![Codacy Badge](https://api.codacy.com/project/badge/Grade/3749bb4e09c74f6785177d318bb8ba15)](https://app.codacy.com/manual/terens.t17/tnt?utm_source=github.com&utm_medium=referral&utm_content=TerensTare/tnt&utm_campaign=Badge_Grade_Dashboard)

## Documentation

WIP

## Getting started

### Option 1: Building the engine from source

#### Prerequisites (aka getting the dependencies)

The project uses [vcpkg](https://github.com/microsoft/vcpkg) to handle the dependencies as vcpkg is cross-platform, and CMake to build the code, so before you go to the next section, make sure you have installed vcpkg and [CMake](https://cmake.org) (at least version 3.10) and set the `VCPKG_DEFAULT_TRIPLET` environmental variable.

*Note*: As you may have noticed on CMakeLists.txt, the minimum supported CMake version is 3.10. I haven't tried compiling with older versions, but if you change (or remove) line 6 of [CMakeLists.txt](./CMakeLists.txt), it will probably work until a certain version. If you try it and it works, please open an [issue](https://github.com/TerensTare/tnt/issues) by writing: 
The project compiles with CMake vx.x.x.(or whatever you want).

*Note*: If you want to build for x86, set `VCPKG_DEFAULT_TRIPLET` as `x86-{yourTargetOS}`. Otherwise, define it as `x64-{yourTargetOS}`.
After you installed vcpkg and CMake, and set `VCPKG_DEFAULT_TRIPLET` please run:
`vcpkg install sdl2 sdl2-image sdl2-image[libjpeg-turbo] sdl2-image[tiff] sdl2-image[libwebp] sdl2-mixer sdl2-mixer[dynamic-load] sdl2-mixer[libflac] sdl2-mixer[mpg123] sdl2-mixer[libmodplug] sdl2-mixer[libvorbis] sdl2-mixer[opusfile] sdl2-ttf nlohmann-json sol2 fmt`.


#### Running the build commands.

At the time of speaking, the engine could be used by including header files and adding source files(.cpp) to your compile command. But there is planned support for building the engine as Windows DLL's and C++20 modules. Until then, the build command is as follows:
``` batch
mkdir build
cd build
cmake ..
```

*Note*: If you are using Microsoft's Visual C++ compiler with CMake (ex. on Visual Studio), you should run the following commands, otherwise, skip this step.
``` batch
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

The main.cpp demo uses some assets located on the `assets` folder. After building, you should copy them to the path where the exe is built.

### Option 2: getting the pre-built binaries.

WIP

### Building the test and samples

WIP

## Contributing

Please see [CONTRIBUTING.md](./CONTRIBUTING.md) to learn how you can contribute to this project.
