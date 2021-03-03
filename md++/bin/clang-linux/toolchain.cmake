set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

add_compile_options(-Wall -Wextra)
add_compile_options(
	"$<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-std=c++17>"
	"$<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,C>:-std=c99>"
)
add_compile_options(
	"$<$<CONFIG:RELEASE>:-O3>"
	"$<$<CONFIG:RELEASE>:-ffunction-sections>"
	"$<$<CONFIG:RELEASE>:-fdata-sections>"
	"$<$<CONFIG:RELEASE>:-flto>"
	"$<$<CONFIG:DEBUG>:-O0>"
	"$<$<CONFIG:DEBUG>:-g>"
)
add_link_options(
	"$<$<CONFIG:RELEASE>:-Wl,--gc-sections>"
	"-Wl,-rpath='${CMAKE_BINARY_DIR}/out/'"
	"-L'${CMAKE_BINARY_DIR}/out'"
)
add_link_options(
	-ldl -lpthread
)
