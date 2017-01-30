
# === CMake lists for "GUI Tests" - (17/11/2014) ===

add_executable(
	TestGUI
	tests/GUI/main.cpp
)

target_link_libraries(TestGUI ForkENGINE)
set_target_properties(TestGUI PROPERTIES DEBUG_POSTFIX "D")
