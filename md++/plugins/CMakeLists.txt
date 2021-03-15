cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PLUGIN_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_DIR}")

include_directories(
	"${CMAKE_SOURCE_DIR}/${project}/includes/"
)

add_subdirectory(terminal)
add_subdirectory(code-block)
add_subdirectory(tuacm)
