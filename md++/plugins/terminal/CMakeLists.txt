cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR}")

include_directories(
	"${CMAKE_SOURCE_DIR}/${project}/includes/"
)

add_library(terminal SHARED
	terminal.cpp
)

target_link_libraries(terminal mdxx_interface)

install(TARGETS terminal DESTINATION "${PLUGIN_DIR}")
