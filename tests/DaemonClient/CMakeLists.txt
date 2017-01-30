
# === CMake lists for "Daemon Client Tests" - (23/09/2014) ===

add_executable(
	TestDaemonClient
	tests/DaemonClient/main.cpp
)

target_link_libraries(TestDaemonClient ForkCore ForkNetwork)
set_target_properties(TestDaemonClient PROPERTIES DEBUG_POSTFIX "D")
