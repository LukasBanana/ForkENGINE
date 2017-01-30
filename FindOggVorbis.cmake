
# Custom CMake module for finding "Ogg Vorbis" files
# Written by Lukas Hermanns on 30/11/2014

# Macros

macro(_OGGVORBIS_APPEND_LIBRARIES _list _release)
   set(_debug ${_release}_DEBUG)
   if(${_debug})
      set(${_list} ${${_list}} optimized ${${_release}} debug ${${_debug}})
   else()
      set(${_list} ${${_list}} ${${_release}})
   endif()
endmacro()

# Find library

find_path(OGG_INCLUDE_DIR ogg/ogg.h)
find_path(VORBIS_INCLUDE_DIR vorbis/vorbisfile.h)

find_library(OGG_LIBRARY NAMES ogg)
find_library(OGG_LIBRARY_DEBUG NAMES ogg)
find_library(VORBIS_LIBRARY NAMES vorbis)
find_library(VORBIS_LIBRARY_DEBUG NAMES vorbis)
find_library(VORBISFILE_LIBRARY NAMES vorbisfile)
find_library(VORBISFILE_LIBRARY_DEBUG NAMES vorbisfile)

# Setup package handle

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	OggVorbis
	DEFAULT_MSG
	OGG_INCLUDE_DIR
	VORBIS_INCLUDE_DIR
    OGG_LIBRARY
    OGG_LIBRARY_DEBUG
    VORBIS_LIBRARY
    VORBIS_LIBRARY_DEBUG
    VORBISFILE_LIBRARY
    VORBISFILE_LIBRARY_DEBUG
)

if(OGGVORBIS_FOUND)
	set(OggVorbis_FOUND TRUE)
	_OGGVORBIS_APPEND_LIBRARIES(Ogg_LIBRARIES OGG_LIBRARY)
	_OGGVORBIS_APPEND_LIBRARIES(Vorbis_LIBRARIES VORBIS_LIBRARY)
	_OGGVORBIS_APPEND_LIBRARIES(VorbisFile_LIBRARIES VORBISFILE_LIBRARY)
endif(OGGVORBIS_FOUND)
