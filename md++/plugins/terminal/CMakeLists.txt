cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/plugins)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/plugins)

include_directories(
	"${CMAKE_SOURCE_DIR}/${project}/includes/"
)

add_library(terminal SHARED
	terminal.cpp
)
