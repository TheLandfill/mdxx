cmake_minimum_required(VERSION 3.0.0)

include_directories(
	"${CMAKE_SOURCE_DIR}/${project}/includes/"
)

add_library(terminal SHARED
	terminal.cpp
)

target_link_libraries(terminal mdxx_interface)

install(TARGETS terminal DESTINATION "${CMAKE_BINARY_DIR}/out")