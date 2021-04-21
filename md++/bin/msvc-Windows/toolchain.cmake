set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10.0)

set(CMAKE_C_COMPILER MSVC)
set(CMAKE_CXX_COMPILER MSVC)
add_compile_options(/Wall /W3 /std:c++17 /utf-8)
add_compile_options(
	"$<$<CONFIG:RELEASE>:/O2>"
	"$<$<CONFIG:DEBUG>:/Od>"
	"$<$<CONFIG:DEBUG>:/DEBUG:FASTLINK>"
)
add_definitions(
	"-D_CRT_SECURE_NO_WARNINGS"
	"-DNOMINMAX"
)

set(VS_STARTUP_PROJECT INSTALL)
