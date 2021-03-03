cmake_minimum_required(VERSION 3.13.0)
project(md++)

set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendors/)
include_directories(
	"${VENDOR_DIR}/re2/"
	"${VENDOR_DIR}/nlohmann_json_cmake_fetchcontent/include/"
	"${CMAKE_SOURCE_DIR}/mdxx/includes/"
)
file(GLOB VENDOR_LIBRARY_DIRS "${VENDOR_DIR}/libs/*")
link_directories("${VENDOR_LIBRARY_DIRS}")

set(OUT_DIR "${CMAKE_BINARY_DIR}/out")
set(PLUGIN_DIR "${OUT_DIR}/plugins")

add_subdirectory(mdxx)
add_subdirectory(plugins)
add_subdirectory(vendors)
