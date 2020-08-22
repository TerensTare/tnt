# TODO:

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

# Find vcpkg.exe and use its path.
if(WIN32)
    find_file(VCPKG_EXE_PATH vcpkg.exe)
else()
    find_file(VCPKG_EXE_PATH vcpkg)
endif()
message("Vcpkg found at ${VCPKG_EXE_PATH}.")

set(CMAKE_TOOLCHAIN_FILE
    ${VCPKG_EXE_PATH}/../scripts/buildsystems/vcpkg.cmake
    CACHE PATH "TOOLCHAIN")

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

file(GLOB SOURCE_FILES
    src/core/*.cpp src/ecs/*.cpp src/ai/steer/*.cpp src/fileIO/*.cpp
    src/ImGui/*.cpp src/math/*.cpp src/tolua/*.cpp src/utils/*.cpp)

list(FILTER SOURCE_FILES EXCLUDE REGEX "Context.cpp$")
list(FILTER SOURCE_FILES EXCLUDE REGEX "AnimatedSprite.cpp$")
list(FILTER SOURCE_FILES EXCLUDE REGEX "Particle.cpp$")
list(FILTER SOURCE_FILES EXCLUDE REGEX "Scriptable.cpp$")
list(FILTER SOURCE_FILES EXCLUDE REGEX "LuaManager.cpp$")

list(APPEND ${SOURCE_FILES} src/ImGui/ImGui.cpp)

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
else()
    message("Not generating documentation because Doxygen was not found")
endif()

add_executable(TnT main.cpp ${SOURCE_FILES})

set_target_properties(TnT PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(TnT PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(TnT PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(TnT PRIVATE ${LUA_LIBRARIES})

target_include_directories(TnT PRIVATE ${LUA_INCLUDE_DIR})

if(WIN32)
    target_link_libraries(TnT PRIVATE SDL2::SDL2 SDL2::SDL2main)
else()
    target_link_libraries(TnT PRIVATE SDL2::SDL2 SDL2::SDL2-static)
endif()

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
    include(InstallRequiredSystemLibraries)

    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The TnT engine.")
    set(CPACK_PACKAGE_VENDOR "Terens Tare")

    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

    set(CPACK_PACKAGE_VERSION_MAJOR "0")
    set(CPACK_PACKAGE_VERSION_MINOR "1")
    set(CPACK_PACKAGE_VERSION_PATCH "0")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "TnT")
    set(CPACK_PACKAGE_FILE_NAME "TnT")

    set(CPACK_COMPONENTS_ALL binary assets)
    set (CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "TnT engine")
    set (CPACK_COMPONENT_APPLICATIONS_DESCRIPTION
        "A 2d game engine that uses Lua and C++20.")

    if(WIN32 AND NOT UNIX)
        set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/assets\\\\TnT.png")
        set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/assets\\\\TnT.ico")
        set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/assets\\\\TnT.ico")
        set(CPACK_NSIS_INSTALLED_ICON_NAME "${CMAKE_SOURCE_DIR}/assets\\\\TnT.ico")
        set(CPACK_NSIS_DISPLAY_NAME "The TnT engine")
        set(CPACK_NSIS_HELP_LINK "https:\\\\github.com/TerensTare/tnt")
        set(CPACK_NSIS_MODIFY_PATH ON)
    else()
        set(CPACK_STRIP_FILES "build/Release/TnT")
        set(CPACK_SOURCE_STRIP_FILES "")
    endif()

    set(CPACK_PACKAGE_EXECUTABLES "TnT" "assets/TnT.png")

    include(CPack)
endif()