function(build_docs)
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
endfunction()

function(build_installer project_name max min patch)
    include(InstallRequiredSystemLibraries)

    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The TnT engine.")
    set(CPACK_PACKAGE_VENDOR "Terens Tare")

    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

    set(CPACK_PACKAGE_VERSION_MAJOR "${max}")
    set(CPACK_PACKAGE_VERSION_MINOR "${min}")
    set(CPACK_PACKAGE_VERSION_PATCH "${patch}")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "${project_name}")
    set(CPACK_PACKAGE_FILE_NAME "${project_name}")

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
endfunction()

function(setup_target project_name)
    set_target_properties(${project_name} PROPERTIES LINKER_LANGUAGE CXX)

    target_link_libraries(${project_name} PRIVATE ${LUA_LIBRARIES})

    if(WIN32)
        target_link_libraries(${project_name} PRIVATE SDL2::SDL2 SDL2::SDL2main)
    else()
        target_link_libraries(${project_name} PRIVATE SDL2::SDL2 SDL2::SDL2-static)
    endif()

    target_link_libraries(${project_name} PRIVATE
        SDL2::SDL2_image SDL2::SDL2_mixer
        SDL2::SDL2_net SDL2::SDL2_ttf
        sol2::sol2 fmt::fmt fmt::fmt-header-only
        nlohmann_json nlohmann_json::nlohmann_json)

    target_link_directories(${project_name} PRIVATE
        ${VCPKG_ROOT}/installed/${VCPKG_DEFAULT_TRIPLET}/lib)
endfunction()