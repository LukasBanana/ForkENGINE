
# === CMake lists for "RayTracing Tests" - (18/12/2014) ===

add_executable(
	TestRayTracing
	tests/RayTracing/main.cpp
)

target_link_libraries(TestRayTracing ForkENGINE)
set_target_properties(TestRayTracing PROPERTIES DEBUG_POSTFIX "D")
