cmake_minimum_required(VERSION 3.0.0)
set(project md++)
project("${project}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(
	"${CMAKE_SOURCE_DIR}/${project}/includes/"
)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/${project}/src/*.cpp")

file(GLOB MDXX_INTERFACE_SOURCES "${CMAKE_SOURCE_DIR}/${project}/export_src/*.cpp")

add_library(mdxx_interface SHARED
	"${MDXX_INTERFACE_SOURCES}"
)

target_link_libraries(mdxx_interface re2)

add_executable("${project}" "${SOURCES}")
target_link_libraries("${project}" mdxx_interface)
