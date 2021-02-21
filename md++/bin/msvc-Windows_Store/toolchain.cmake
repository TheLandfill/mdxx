set(CMAKE_SYSTEM_NAME WindowsStore)
set(CMAKE_SYSTEM_VERSION 10.0)

set(CMAKE_C_COMPILER MSVC)
set(CMAKE_CXX_COMPILER MSVC)
add_compile_options(/Wall /W4 /std:c++17)
add_compile_options(
	"$<$<CONFIG:RELEASE>:/O2>"
	"$<$<CONFIG:DEBUG>:/Od>"
	"$<$<CONFIG:DEBUG>:/DEBUG:FASTLINK>"
)
add_definitions(
	"-D_CRT_SECURE_NO_WARNINGS"
)

set(VS_STARTUP_PROJECT md++)
