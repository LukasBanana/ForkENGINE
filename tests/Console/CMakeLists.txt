
# === CMake lists for "Console Tests" - (25/02/2014) ===

add_executable(
	TestConsole
	tests/Console/main.cpp
)

target_link_libraries(TestConsole ForkCore)
set_target_properties(TestConsole PROPERTIES DEBUG_POSTFIX "D")
