
# === CMake lists for "Scene Tests" - (12/03/2014) ===

add_executable(
	TestScene
	tests/Scene/main.cpp
)

target_link_libraries(TestScene ForkENGINE)
set_target_properties(TestScene PROPERTIES DEBUG_POSTFIX "D")
