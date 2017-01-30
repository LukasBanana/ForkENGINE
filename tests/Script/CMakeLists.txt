
# === CMake lists for "Script Tests" - (28/09/2014) ===

add_executable(
	TestScript
	tests/Script/main.cpp
)

target_link_libraries(TestScript ForkENGINE)
set_target_properties(TestScript PROPERTIES DEBUG_POSTFIX "D")
