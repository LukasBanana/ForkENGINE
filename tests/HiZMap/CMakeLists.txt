
# === CMake lists for "Hi-Z Map Tests" - (13/07/2014) ===

add_executable(
	TestHiZMap
	tests/HiZMap/main.cpp
)

target_link_libraries(TestHiZMap ForkENGINE)
set_target_properties(TestHiZMap PROPERTIES DEBUG_POSTFIX "D")
