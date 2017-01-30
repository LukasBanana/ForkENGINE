
# Custom CMake module for finding "Newton Game Dynamics" files
# Written by Lukas Hermanns on 18/04/2014

set(Newton_ROOT_DIR ${PROJECT_SOURCE_DIR}/../newton-dynamics)

# Macros

macro(_NEWTON_APPEND_LIBRARIES _list _release)
   set(_debug ${_release}_DEBUG)
   if(${_debug})
      set(${_list} ${${_list}} optimized ${${_release}} debug ${${_debug}})
   else()
      set(${_list} ${${_list}} ${${_release}})
   endif()
endmacro()

# Find library

find_path(
	Newton_INCLUDE_DIR
	NAMES
		Newton.h
	HINTS
		${Newton_ROOT_DIR}/include
)

find_library(
	Newton_LIBRARY
	NAMES
		newton
	HINTS
		${Newton_ROOT_DIR}/lib
)

find_library(
	Newton_LIBRARY_DEBUG
	NAMES
		newton_d
	HINTS
		${Newton_ROOT_DIR}/lib
)

# Setup package handle

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	Newton
	DEFAULT_MSG
	Newton_INCLUDE_DIR
    Newton_LIBRARY
    Newton_LIBRARY_DEBUG
)

if(NEWTON_FOUND)
	set(Newton_FOUND TRUE)
	_NEWTON_APPEND_LIBRARIES(Newton_LIBRARIES Newton_LIBRARY)
endif(NEWTON_FOUND)
