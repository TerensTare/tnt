
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was TnTConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

find_dependency(Lua REQUIRED)
find_dependency(SDL2 CONFIG REQUIRED)
find_dependency(sdl2-image CONFIG REQUIRED)
find_dependency(sdl2-mixer CONFIG REQUIRED)
find_dependency(sdl2-ttf CONFIG REQUIRED)
find_dependency(sdl2-net CONFIG REQUIRED)
find_dependency(sol2 CONFIG REQUIRED)
find_dependency(fmt CONFIG REQUIRED)
find_dependency(nlohmann_json CONFIG REQUIRED)

include(${CMAKE_CURRENT_LIST_DIR}/TnTTargets.cmake)
check_required_components(TnT)
