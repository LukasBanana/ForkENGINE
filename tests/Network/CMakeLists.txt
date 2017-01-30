
# === CMake lists for "Network Tests" - (30/03/2014) ===

add_executable(
	TestNetwork
	tests/Network/main.cpp
)

target_link_libraries(TestNetwork ForkCore ForkNetwork)
set_target_properties(TestNetwork PROPERTIES DEBUG_POSTFIX "D")
