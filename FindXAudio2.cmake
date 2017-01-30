
# Custom CMake module for finding "XAudio2" files

if(WIN32)
	find_path(DirectX_INCLUDE_DIRS xaudio2.h PATHS
		"$ENV{DXSDK_DIR}/Include"
		"$ENV{PROGRAMFILES}/Microsoft DirectX SDK*/Include"
	)
	get_filename_component(DirectX_ROOT_DIR "${DirectX_INCLUDE_DIRS}/.." ABSOLUTE)
	
	if(CMAKE_CL_64)
		set(DirectX_LIBRARY_PATHS "${DirectX_ROOT_DIR}/Lib/x64")
	else(CMAKE_CL_64)
		set(DirectX_LIBRARY_PATHS "${DirectX_ROOT_DIR}/Lib/x86" "${DirectX_ROOT_DIR}/Lib")
	endif(CMAKE_CL_64)
	
	find_library(DirectX_X3DAUDIO_LIBRARY X3DAudio ${DirectX_LIBRARY_PATHS} NO_DEFAULT_PATH)
	find_library(DirectX_XAPOFX_LIBRARY xapofx ${DirectX_LIBRARY_PATHS} NO_DEFAULT_PATH)
	
	set(XAUDIO2_LIBRARIES ${DirectX_X3DAUDIO_LIBRARY} ${DirectX_XAPOFX_LIBRARY})
	
	include(FindPackageHandleStandardArgs)
	FIND_PACKAGE_HANDLE_STANDARD_ARGS(DirectX DEFAULT_MSG DirectX_ROOT_DIR XAUDIO2_LIBRARIES DirectX_INCLUDE_DIRS)
	
	mark_as_advanced(
		DirectX_INCLUDE_DIRS
		DirectX_X3DAUDIO_LIBRARY
		DirectX_XAPOFX_LIBRARY
	)
endif(WIN32)
