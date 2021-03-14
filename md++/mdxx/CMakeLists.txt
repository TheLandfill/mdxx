cmake_minimum_required(VERSION 3.0.0)

find_package(PythonLibs REQUIRED)
add_library(py STATIC IMPORTED)
if (WIN32)
set_property(TARGET py PROPERTY IMPORTED_LOCATION_DEBUG "${PYTHON_DEBUG_LIBRARY}")
set_property(TARGET py PROPERTY IMPORTED_LOCATION_RELEASE "${PYTHON_LIBRARY_RELEASE}")
else()
set_property(TARGET py PROPERTY IMPORTED_LOCATION "${PYTHON_LIBRARY_RELEASE}")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR}")

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

add_library(mdxx_interface SHARED
	"${MDXX_INTERFACE_SOURCES}"
)

target_link_libraries(mdxx_interface re2 py mdxx_util)

if (NOT WIN32)
	target_link_libraries(mdxx_interface dl)
endif (NOT WIN32)

add_executable("${project}" "${SOURCES}")
target_link_libraries("${project}" mdxx_interface)
target_compile_definitions(mdxx_interface PRIVATE MDXX_INTERFACE_DLL)
if (WIN32)
	install(TARGETS "${project}" mdxx_interface DESTINATION "${OUT_DIR}")
endif (WIN32)
