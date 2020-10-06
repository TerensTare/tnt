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

### Running the build commands.

At the time of speaking, the engine can be used by including header files and adding source files(.cpp) to your compile command. But there is planned support for building the engine as Windows DLL's and C++20 modules. Until then, the build command is as follows:
``` cmd
mkdir build
cd build
cmake ..
cmake --build .
```

### Building the documentation

The documentation is built by default when building with CMake. To build the documentation, you need to have doxygen installed, which can be obtained [here](http://www.doxygen.nl/manual/install.html). To avoid building the documentation, please run `cmake -DBUILD_DOCS=OFF ..` instead of just `cmake ..`.

## Option 2: getting the pre-built binaries.

WIP