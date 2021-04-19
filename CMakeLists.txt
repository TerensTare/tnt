# TODO: testing, examples flags for x64/x86, debug/release. install the editor and assets.

# The project uses C++20, which is minimally supported by CMake 3.12.4
cmake_minimum_required(VERSION 3.12.4 FATAL_ERROR)

# ######################################################################################################################################################################################################
# Vcpkg stuff #
# ######################################################################################################################################################################################################

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
        "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
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

# ######################################################################################################################################################################################################
# The Dynamic Library #
# ######################################################################################################################################################################################################

project(
    TnT
    VERSION 0.1.0
    DESCRIPTION "The TnT engine SDK"
    HOMEPAGE_URL "https://github.com/TerensTare/tnt"
    LANGUAGES CXX)

set(TNT_BUILD ON)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "CMake should not be used to build TnT in a directory that has a CMakeLists.txt file!!\n
        Please consider making a \"build\" subdirectory and run cmake .. from the \"build\" subdirectory.")
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

if(NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "")
endif()

list(APPEND CMAKE_PREFIX_PATH $ENV{VCPKG_ROOT}/installed/$ENV{VCPKG_DEFAULT_TRIPLET})

add_library(project_config INTERFACE)
target_compile_features(project_config INTERFACE cxx_std_20)

include(cmake/StandardProjectSettings.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)
include(cmake/StaticAnalyzers.cmake)

add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)
enable_sanitizers(project_config)

# ######################################################################################################################################################################################################
# Documentation #
# ######################################################################################################################################################################################################

include(cmake/Doxygen.cmake)
build_docs()

# ######################################################################################################################################################################################################
# Find the dependecies #
# ######################################################################################################################################################################################################

include(FindLua)

find_package(Lua REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-mixer CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)
find_package(sdl2-net CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# ######################################################################################################################################################################################################
# Build the library #
# ######################################################################################################################################################################################################

file(
    GLOB
    SOURCE_FILES
    src/core/*.cpp
    src/fileIO/*.cpp
    src/ImGui/ImGui.cpp
    src/json/*.cpp
    src/ldtk/*.cpp
    src/math/*.cpp
    src/pcg/*.cpp
    src/tolua/*.cpp
    src/utils/Benchmark.cpp
    $<DEFINED
    ${TNT_DO_EXPERIMENTS}>:src/exp/Runtimer.cpp)

list(FILTER SOURCE_FILES EXCLUDE REGEX "LuaDooEcs.cpp$")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
add_library(TnT::TnT ALIAS TnT)

target_compile_definitions(
    ${PROJECT_NAME} PUBLIC TNT_BUILD SOL_PRINT_ERRORS=1 SOL_NO_EXCEPTIONS=1 $<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:TNT_PROFILING # When in Debug or RelWithDebInfo build, enable profiling
                           FMT_COMPILE_TIME_CHECKS=1)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES SOVERSION 0.1
               VERSION ${PROJECT_VERSION}
               LINKER_LANGUAGE CXX)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC SDL2::SDL2
           SDL2::SDL2main
           SDL2::SDL2_image
           SDL2::SDL2_mixer
           SDL2::SDL2_net
           SDL2::SDL2_ttf
           ${LUA_LIBRARIES}
           sol2::sol2
           fmt::fmt
           nlohmann_json
           nlohmann_json::nlohmann_json
           # utility targets
    PRIVATE project_warnings project_config)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE ${LUA_INCLUDE_DIR})

if(MSVC)
    target_compile_options(TnT PRIVATE /fp:fast)

    # Add tnt.natvis to the build
    set(TNT_ADD_NATVIS TRUE)
    target_sources(${PROJECT_NAME} INTERFACE $<INSTALL_INTERFACE:extra/tnt.natvis> $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extra/tnt.natvis>)
else()
    target_compile_options(TnT PRIVATE --ffast-math)
endif()

if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_INSTALL_PREFIX "C:/Program Files/TnT")
endif()

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake)

if(${CMAKE_VERSION} VERSION_LESS "3.14.0")
    install(
        TARGETS ${PROJECT_NAME}
        EXPORT TnTTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
else()
    install(TARGETS ${PROJECT_NAME} EXPORT TnTTargets) # A shorthand CMake 3.14 feature
endif()

install(
    EXPORT TnTTargets
    FILE TnTTargets.cmake
    NAMESPACE TnT::
    DESTINATION ${INSTALL_CONFIGDIR})

install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    FILES_MATCHING
    PATTERN "*.hpp")

if(TNT_ADD_NATVIS)
    install(FILES extra/tnt.natvis DESTINATION .)
endif()

# ######################################################################################################################################################################################################
# TnTConfigVersion.cmake stuff #
# ######################################################################################################################################################################################################

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/TnTConfig.cmake.in ${CMAKE_CURRENT_LIST_DIR}/cmake/TnTConfig.cmake INSTALL_DESTINATION ${INSTALL_CONFIGDIR})

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/TnTConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(FILES ${CMAKE_CURRENT_LIST_DIR}/cmake/TnTConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/TnTConfigVersion.cmake DESTINATION ${INSTALL_CONFIGDIR})

export(PACKAGE TnT)

# ######################################################################################################################################################################################################
# The editor #
# ######################################################################################################################################################################################################

option(BUILD_EDITOR "Build TnT's visual editor" ON)

if(BUILD_EDITOR)
    message("Building the visual editor of TnT.")

    if(TNT_BUILD)
        unset(TNT_BUILD)
    endif()

    file(GLOB EXE_SOURCES src/doo_ecs/*.cpp)

    find_package(unofficial-nativefiledialog CONFIG REQUIRED)

    add_executable(TnTEditor main.cpp ${EXE_SOURCES} src/tolua/LuaDooEcs.cpp)

    if(MSVC)
        target_compile_options(TnTEditor PRIVATE /fp:fast)
    else()
        target_compile_options(TnTEditor PRIVATE --ffast-math)
    endif()

    target_link_libraries(
        TnTEditor
        PRIVATE TnT::TnT
                unofficial::nativefiledialog::nfd
                # utility targets
                project_warnings
                project_config)

    install(TARGETS TnTEditor)
endif()
