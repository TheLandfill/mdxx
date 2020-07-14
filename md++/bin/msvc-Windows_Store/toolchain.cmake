set(CMAKE_SYSTEM_NAME WindowsStore)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_VERSION 10)

set(CMAKE_C_COMPILER MSVC)
set(CMAKE_CXX_COMPILER MSVC)
add_compile_options(/Wall)
add_compile_options(
	"$<$<CONFIG:RELEASE>:/O2>"
	"$<$<CONFIG:DEBUG>:/Od>"
	"$<$<CONFIG:DEBUG>:/DEBUG:FASTLINK>"
)
add_link_options(
	"$<$<CONFIG:RELEASE>:/GL"
)

set(VS_STARTUP_PROJECT game)
