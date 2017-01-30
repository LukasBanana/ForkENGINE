
# === CMake lists for "RenderTargets Tests" - (25/04/2014) ===

add_executable(
	TestRenderTargets
	tests/RenderTargets/main.cpp
)

target_link_libraries(TestRenderTargets ForkENGINE)
set_target_properties(TestRenderTargets PROPERTIES DEBUG_POSTFIX "D")
