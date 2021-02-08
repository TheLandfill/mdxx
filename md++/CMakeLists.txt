cmake_minimum_required(VERSION 3.13.0)
project(md++)

set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendors/)
include_directories(
	"${VENDOR_DIR}/re2/"
	"${VENDOR_DIR}/nlohmann_json_cmake_fetchcontent/include/"
	"${CMAKE_SOURCE_DIR}/md++/includes/"
)
file(GLOB VENDOR_LIBRARY_DIRS "${VENDOR_DIR}/libs/*")
link_directories("${VENDOR_LIBRARY_DIRS}")

add_subdirectory(md++)
add_subdirectory(plugins)
add_subdirectory(vendors)
