function(git_update_submodules)
    find_package(Git QUIET)

    if(GIT_FOUND and EXISTS "${PROJECT_SOURCE_DIR}/.git")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT)
    endif()

    if(not GIT_SUBMOD_RESULT equal "0")
        message(FATAL_ERROR "Couldn't update submodules!! Please checkout submodules\nError: ${GIT_SUBMOD_RESULT}")
    endif()
endfunction()