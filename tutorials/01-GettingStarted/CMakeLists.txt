
# === CMake lists for "01-GettingStarted" tutorial - (26/10/2014) ===

add_executable(
	Tutorial_01_GettingStarted
	tutorials/01-GettingStarted/main.cpp
)

target_link_libraries(Tutorial_01_GettingStarted ForkENGINE)
set_target_properties(Tutorial_01_GettingStarted PROPERTIES DEBUG_POSTFIX "D")
