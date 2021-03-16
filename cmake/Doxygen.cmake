function(build_docs)
    option(BUILD_DOCS "Build TnT's documentation" ON)
    if(BUILD_DOCS)
        find_package(Doxygen)
        if(DOXYGEN_FOUND)
            set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/Doxyfile.in)
            set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxyfile)
            set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/include)
            set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)
            file(WRITE ${DOXYGEN_OUT} "")
            configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
            add_custom_target(
                DOCUMENTATION ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs
                COMMENT "Generating documentation with Doxygen"
                VERBATIM)
        else()
            message("Not generating documentation because Doxygen was not found")
        endif()
    endif()
endfunction()