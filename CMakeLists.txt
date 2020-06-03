# TODO:
# installer
# testing
# flags for x64/x86, debug/release.

# The project uses C++20, which is minimally supported by CMake 3.12.4
cmake_minimum_required(VERSION 3.12.4 FATAL_ERROR)
set(PROJECT_NAME "TnT")
project(${PROJECT_NAME}
    VERSION 0.1.0
    DESCRIPTION "A 2d game engine written using C++20 features"
    HOMEPAGE_URL "https://terenstare.github.io/tnt/"
    LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Remove the /RTC* flags in case of compiling with MSVC.
foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    STRING (REGEX REPLACE "/RTC[^ ]*" "" ${flag_var} "${${flag_var}}")
endforeach(flag_var)

set(CMAKE_TOOLCHAIN_FILE
    ${VCPKG_EXE_PATH}/../scripts/buildsystems/vcpkg.cmake
    CACHE PATH "TOOLCHAIN")

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if (EXISTS "${LOC_PATH}")
message(FATAL_ERROR
    "CMake should not be used to build TnT in a directory that has a CMakeLists.txt file!!\n
    Please consider making a \"build\" subdirectory and run cmake .. from the \"build\" directory.")
endif()

# Copy assets folder to build directory
if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/assets)
    file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/assets)
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

if(MSVC)
    set(COMMON_COMPILE_OPTIONS /EHsc /O2)
endif()
set(TNT_COMPILE_OPTIONS ${COMMON_COMPILE_OPTIONS})

include(FindLua)
include(cmake/StandardProjectSettings.cmake)

# add_library(project_options INTERFACE)
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
if(WIN32)
    find_file(VCPKG_EXE_PATH vcpkg.exe)
else()
    find_file(VCPKG_EXE_PATH vcpkg)
endif(WIN32)
message("Vcpkg found at ${VCPKG_EXE_PATH}.")


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
# find_package(sdl2-gfx CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

configure_file(
    "${PROJECT_SOURCE_DIR}/include/version.h.in"
    "${PROJECT_BINARY_DIR}/include/version.h")

set(SOURCE_FILES src/core/Window.cpp src/core/Input.cpp
    src/core/Camera.cpp src/core/Space.cpp src/core/Scene.cpp
    src/math/Rectangle.cpp src/fileIO/AssetManager.cpp
    src/ecs/Object.cpp src/ecs/Sprite.cpp src/ecs/RigidBody.cpp
    src/ImGui/ImGui.cpp src/utils/Timer.cpp)

if (TNT_DO_EXPERIMENTS)
    list(APPEND SOURCE_FILES exp/Runtimer.cpp)
endif()

# documentation stuff
option(BUILD_DOCS "Build TnT documentation" ON)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxyfile)
    set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/include)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)
    file(WRITE ${DOXYGEN_OUT} "")
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    add_custom_target(DOCUMENTATION ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs
        COMMENT "Generating documentation with Doxygen"
        VERBATIM)
else(DOXYGEN_FOUND)
    message("Doxygen must be installed to build the documentation")
endif(DOXYGEN_FOUND)


add_executable(TnT main.cpp ${SOURCE_FILES})

set_target_properties(TnT PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(TnT PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(TnT PRIVATE ${LUA_LIBRARIES})

target_include_directories(TnT PRIVATE ${LUA_INCLUDE_DIR})

if(WIN32)
    target_link_libraries(TnT PRIVATE SDL2::SDL2 SDL2::SDL2main)
else()
    target_link_libraries(TnT PRIVATE SDL2::SDL2 SDL2::SDL2-static)
endif(WIN32)

# target_link_libraries(TnT PRIVATE SDL2::SDL2_gfx)
target_link_libraries(TnT PRIVATE SDL2::SDL2_image)
target_link_libraries(TnT PRIVATE SDL2::SDL2_mixer)
target_link_libraries(TnT PRIVATE SDL2::SDL2_net)
target_link_libraries(TnT PRIVATE SDL2::SDL2_ttf)
target_link_libraries(TnT PRIVATE sol2::sol2)
target_link_libraries(TnT PRIVATE fmt::fmt fmt::fmt-header-only)
target_link_libraries(TnT PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

target_link_directories(TnT PRIVATE
    ${VCPKG_EXE_PATH}/../installed/${VCPKG_DEFAULT_TRIPLET}/lib)
target_compile_options(TnT PRIVATE ${TNT_COMPILE_OPTIONS})

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

# install(TARGETS ${PROJ_NAME} DESTINATION "${${PROJ_NAME}_PATH_INSTALL}/lib/${CMAKE_BUILD_TYPE}/")
