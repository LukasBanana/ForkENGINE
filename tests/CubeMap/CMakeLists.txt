
# === CMake lists for "CubeMap Tests" - (23/08/2014) ===

add_executable(
	TestCubeMap
	tests/CubeMap/main.cpp
)

target_link_libraries(TestCubeMap ForkENGINE)
set_target_properties(TestCubeMap PROPERTIES DEBUG_POSTFIX "D")
