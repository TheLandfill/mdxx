cmake_minimum_required(VERSION 3.0.0)
set(project mdxx)
project("${project}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR}")

include_directories(
	"${CMAKE_SOURCE_DIR}/${project}/includes/"
)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/${project}/program_src/*.cpp")

file(GLOB MDXX_INTERFACE_SOURCES "${CMAKE_SOURCE_DIR}/${project}/src/*.cpp")

add_library(mdxx_interface SHARED
	"${MDXX_INTERFACE_SOURCES}"
)

target_link_libraries(mdxx_interface re2)

if (NOT WIN32)
	target_link_libraries(mdxx_interface dl)
endif (NOT WIN32)

add_executable("${project}" "${SOURCES}")
target_link_libraries("${project}" mdxx_interface)
target_compile_definitions(mdxx_interface PRIVATE MDXX_INTERFACE_DLL)
if (WIN32)
	install(TARGETS "${project}" mdxx_interface DESTINATION "${OUT_DIR}")
endif (WIN32)