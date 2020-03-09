# TODO:
# installer
# testing
# flags for x64/x86, debug/release.

cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME "TnT")
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_TOOLCHAIN_FILE 
    ${VCPKG_EXE_PATH}/scripts/buildsystems/vcpkg.cmake
    CACHE PATH "TOOLCHAIN")

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if (EXISTS "${LOC_PATH}")
message(FATAL_ERROR
    "CMake should not be used to build TnT in a directory that has a CMakeLists.txt file!!\n
    Please consider making a \"build\" subdirectory and run cmake .. from the \"build\" directory.")
endif()

include(cmake/StandardProjectSettings.cmake)

add_library(project_options INTERFACE)
# target_compile_features(project_options INTERFACE cxx_std_17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)
include(cmake/StaticAnalyzers.cmake)

# Find vcpkg.exe and use it's path.
find_file(VCPKG_EXE_PATH vcpkg.exe)
include_directories("${VCPKG_EXE_PATH}/installed/${VCPKG_DEFAULT_TRIPLET}/include")

list(APPEND CMAKE_PREFIX_PATH
    ${VCPKG_EXE_PATH}/installed/${VCPKG_DEFAULT_TRIPLET}/share/sdl2
    ${VCPKG_EXE_PATH}/installed/${VCPKG_DEFAULT_TRIPLET}/share/sdl2-gfx
    ${VCPKG_EXE_PATH}/installed/${VCPKG_DEFAULT_TRIPLET}/share/sdl2-image
    ${VCPKG_EXE_PATH}/installed/${VCPKG_DEFAULT_TRIPLET}/share/sdl2-mixer
    ${VCPKG_EXE_PATH}/installed/${VCPKG_DEFAULT_TRIPLET}/share/sdl2-net
    ${VCPKG_EXE_PATH}/installed/${VCPKG_DEFAULT_TRIPLET}/share/sol2
    ${VCPKG_EXE_PATH}/installed/${VCPKG_DEFAULT_TRIPLET}/share/fmt
    ${VCPKG_EXE_PATH}/installed/${VCPKG_DEFAULT_TRIPLET}/share/nlohmann_json
)

# Load required packages.
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-gfx CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-mixer CONFIG REQUIRED)
find_package(sdl2-net CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

configure_file(
    "${PROJECT_SOURCE_DIR}/include/version.h.in"
    "${PROJECT_BINARY_DIR}/include/version.h")

# Handle build type.
set(default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Build the TnT Engine in \"${default_build_type}\" mode.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" cache string "Choose the type of build." FORCE)
    set_property(cache CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include_directories(include)
add_subdirectory(src)

if(NOT TNT_UNIT_BUILD)
    list(APPEND SOURCE_FILES main.cpp)
endif()

add_executable(TnT ${SOURCE_FILES})

target_link_libraries(TnT PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(TnT PRIVATE SDL2::SDL2_gfx)
target_link_libraries(TnT PRIVATE SDL2::SDL2_image)
target_link_libraries(TnT PRIVATE SDL2::SDL2_mixer)
target_link_libraries(TnT PRIVATE SDL2::SDL2_net)
target_link_libraries(TnT PRIVATE sol2::sol2)
target_link_libraries(TnT PRIVATE fmt::fmt fmt::fmt-header-only)
target_link_libraries(TnT PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

# install(TARGETS ${PROJ_NAME} DESTINATION "${${PROJ_NAME}_PATH_INSTALL}/lib/${CMAKE_BUILD_TYPE}/")