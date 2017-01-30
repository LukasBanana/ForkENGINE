
# === CMake lists for "SSCT Tests" - (05/10/2014) ===

add_executable(
	TestSSCT
	tests/SSCT/main.cpp
)

target_link_libraries(TestSSCT ForkENGINE)
set_target_properties(TestSSCT PROPERTIES DEBUG_POSTFIX "D")
