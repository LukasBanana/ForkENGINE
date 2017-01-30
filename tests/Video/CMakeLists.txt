
# === CMake lists for "Video Tests" - (02/03/2014) ===

add_executable(
	TestVideo
	tests/Video/main.cpp
)

target_link_libraries(TestVideo ForkENGINE)
set_target_properties(TestVideo PROPERTIES DEBUG_POSTFIX "D")
