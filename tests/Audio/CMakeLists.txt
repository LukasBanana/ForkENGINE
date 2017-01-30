
# === CMake lists for "Audio Tests" - (25/11/2014) ===

add_executable(
	TestAudio
	tests/Audio/main.cpp
)

target_link_libraries(TestAudio ForkENGINE)
set_target_properties(TestAudio PROPERTIES DEBUG_POSTFIX "D")
