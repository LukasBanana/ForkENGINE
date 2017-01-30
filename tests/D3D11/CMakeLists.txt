
# === CMake lists for "D3D11 Tests" - (20/07/2014) ===

add_executable(
	TestD3D11
	tests/D3D11/main.cpp
)

target_link_libraries(TestD3D11 ForkENGINE)
set_target_properties(TestD3D11 PROPERTIES DEBUG_POSTFIX "D")
