
# === CMake lists for "Physics Tests" - (18/04/2014) ===

add_executable(
	TestPhysics
	tests/Physics/main.cpp
)

target_link_libraries(TestPhysics ForkENGINE)
set_target_properties(TestPhysics PROPERTIES DEBUG_POSTFIX "D")
