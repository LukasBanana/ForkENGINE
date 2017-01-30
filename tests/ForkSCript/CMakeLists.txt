
# === CMake lists for "ForkSCript Tests" - (28/02/2014) ===

add_executable(
	TestForkSCript
	tests/ForkSCript/main.cpp
)

target_link_libraries(TestForkSCript ForkCore)
set_target_properties(TestForkSCript PROPERTIES DEBUG_POSTFIX "D")
