# TODO:

# testing
# flags for x64/x86, debug/release.

# The project uses C++20, which is minimally supported by CMake 3.12.4
cmake_minimum_required(VERSION 3.12.4 FATAL_ERROR)
set(PROJECT_NAME "TnT")

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()

if(NOT DEFINED ENV{VCPKG_FEATURE_FLAGS})
    set(ENV{VCPKG_FEATURE_FLAGS} "manifests,binarycaching")
endif()

if(NOT DEFINED ENV{VCPKG_DEFAULT_TRIPLET})
    if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bit os
        if(WIN32)
            set(ENV{VCPKG_DEFAULT_TRIPLET} "x64-windows")
        elseif(UNIX AND NOT APPLE)
            set(ENV{VCPKG_DEFAULT_TRIPLET} "x64-linux")
        elseif(APPLE)
            set(ENV{VCPKG_DEFAULT_TRIPLET} "x64-osx")
        endif()
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4) # 32 bit os
        if(WIN32)
            set(ENV{VCPKG_DEFAULT_TRIPLET} "x86-windows")
        elseif(UNIX AND NOT APPLE)
            set(ENV{VCPKG_DEFAULT_TRIPLET} "x86-linux")
        elseif(APPLE)
            set(ENV{VCPKG_DEFAULT_TRIPLET} "x86-osx")
        endif()
    endif()
endif()

project(${PROJECT_NAME}
    VERSION 0.1.0
    DESCRIPTION "A 2d game engine written using C++20 features"
    HOMEPAGE_URL "https://terenstare.github.io/tnt/"
    LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if (EXISTS "${LOC_PATH}")
    message(FATAL_ERROR
        "CMake should not be used to build TnT in a directory that has a CMakeLists.txt file!!\n
        Please consider making a \"build\" subdirectory and run cmake .. from the \"build\" subdirectory.")
endif()

include(FindLua)
include(cmake/StandardProjectSettings.cmake)

# add_library(project_options INTERFACE)
# target_compile_features(project_options INTERFACE cxx_std_20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)
include(cmake/StaticAnalyzers.cmake)

# if(MSVC)
#     list(APPEND CMAKE_CXX_FLAGS /fp:fast) # /GR- for no rtti
# else()
#     list(APPEND CMAKE_CXX_FLAGS -ffast-math) # -fno-rtti for no rtti
# endif()

if(NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "")
endif()

list(APPEND CMAKE_PREFIX_PATH ${VCPKG_EXE_PATH}/../installed/${VCPKG_DEFAULT_TRIPLET})

# Load required packages.
find_package(Lua REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-mixer CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)
find_package(sdl2-net CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

file(GLOB SOURCE_FILES
    src/core/*.cpp src/doo_ecs/*.cpp src/fileIO/*.cpp
    src/ImGui/ImGui.cpp src/math/*.cpp)

list(FILTER SOURCE_FILES EXCLUDE REGEX "Context.cpp$")
list(FILTER SOURCE_FILES EXCLUDE REGEX "Space.cpp$")
list(FILTER SOURCE_FILES EXCLUDE REGEX "Scene.cpp$")
list(FILTER SOURCE_FILES EXCLUDE REGEX "VirtualFS.cpp$")
list(FILTER SOURCE_FILES EXCLUDE REGEX "LuaManager.cpp$")

if (TNT_DO_EXPERIMENTS)
    list(APPEND SOURCE_FILES exp/Runtimer.cpp)
endif()

# documentation stuff
include(cmake/Setup.cmake)

option(BUILD_DOCS "Build TnT documentation" ON)
if(BUILD_DOCS)
    build_docs()
endif()

add_executable(TnT main.cpp ${SOURCE_FILES})

target_include_directories(TnT PRIVATE ${LUA_INCLUDE_DIR})
target_include_directories(TnT PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(TnT PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
    

setup_target(TnT)

# Copy assets folder to build directory
if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/assets)
    file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/assets)
endif()

install(TARGETS TnT ARCHIVE COMPONENT assets RUNTIME DESTINATION assets)
install(TARGETS TnT ARCHIVE COMPONENT binary RUNTIME DESTINATION .)

set(CMAKE_INSTALL_BINDIR .)

# CPack config stuff
option(BUILD_INSTALLER "Build the installer" OFF)

if(BUILD_INSTALLER)
    build_installer(TnT 0 1 0)
endif()