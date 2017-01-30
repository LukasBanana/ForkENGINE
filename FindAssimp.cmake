
# Custom CMake module for finding "Assimp" files
# Written by Lukas Hermanns on 25/03/2014

set(Assimp_ROOT_DIR ${PROJECT_SOURCE_DIR}/../assimp-3.0)

# Macros

macro(_ASSIMP_APPEND_LIBRARIES _list _release)
   set(_debug ${_release}_DEBUG)
   if(${_debug})
      set(${_list} ${${_list}} optimized ${${_release}} debug ${${_debug}})
   else()
      set(${_list} ${${_list}} ${${_release}})
   endif()
endmacro()

# Find library

find_path(
	Assimp_INCLUDE_DIR
	NAMES
		Importer.hpp
		scene.h
		postprocess.h
	HINTS
		${Assimp_ROOT_DIR}/include/assimp
)

find_library(
	Assimp_LIBRARY
	NAMES
		assimp
	HINTS
		${Assimp_ROOT_DIR}/lib
)

find_library(
	Assimp_LIBRARY_DEBUG
	NAMES
		assimpD
	HINTS
		${Assimp_ROOT_DIR}/lib
)

find_library(
	zlib_LIBRARY
	NAMES
		zlib
	HINTS
		${Assimp_ROOT_DIR}/lib
)

find_library(
	zlib_LIBRARY_DEBUG
	NAMES
		zlibD
	HINTS
		${Assimp_ROOT_DIR}/lib
)

# Setup package handle

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	Assimp
	DEFAULT_MSG
	Assimp_INCLUDE_DIR
    Assimp_LIBRARY
    Assimp_LIBRARY_DEBUG
    zlib_LIBRARY
    zlib_LIBRARY_DEBUG
)

if(ASSIMP_FOUND)
	set(Assimp_FOUND TRUE)
	_ASSIMP_APPEND_LIBRARIES(Assimp_LIBRARIES Assimp_LIBRARY)
	_ASSIMP_APPEND_LIBRARIES(zlib_LIBRARIES zlib_LIBRARY)
endif(ASSIMP_FOUND)
