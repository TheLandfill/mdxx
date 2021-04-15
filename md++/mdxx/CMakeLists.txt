cmake_minimum_required(VERSION 3.0.0)

find_package(PythonLibs REQUIRED)
add_library(py STATIC IMPORTED)
set_property(TARGET py PROPERTY IMPORTED_LOCATION "${PYTHON_LIBRARY_RELEASE}")
if (WIN32)
set_property(TARGET py PROPERTY IMPORTED_LOCATION_DEBUG "${PYTHON_DEBUG_LIBRARY}")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR}/lib$<0:>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR}/lib$<0:>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR}$<0:>")

include_directories(
	"${CMAKE_SOURCE_DIR}/${project}/includes/"
	${PYTHON_INCLUDE_DIRS}
)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/${project}/program_src/*.cpp")

file(GLOB MDXX_INTERFACE_SOURCES "${CMAKE_SOURCE_DIR}/${project}/src/*.cpp")

file(GLOB UTIL_SOURCES "${CMAKE_SOURCE_DIR}/${project}/util/*.cpp")

add_library(mdxx_util STATIC
	"${UTIL_SOURCES}"
)

target_link_libraries(mdxx_util re2)

add_library(mdxx_interface SHARED
	"${MDXX_INTERFACE_SOURCES}"
)

target_link_libraries(mdxx_interface py mdxx_util)

if (NOT WIN32)
	target_link_libraries(mdxx_interface dl)
endif (NOT WIN32)

add_executable("${project}" "${SOURCES}")
target_link_libraries("${project}" mdxx_interface)
target_compile_definitions(mdxx_interface PRIVATE MDXX_INTERFACE_DLL)
