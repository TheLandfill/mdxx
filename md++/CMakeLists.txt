cmake_minimum_required(VERSION 3.13.0)
if (WIN32)
	set(CMAKE_GENERATOR_PLATFORM x64)
endif(WIN32)
project(mdxx)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CDD_STANDARD_REQUIRED ON)
set(project ${CMAKE_PROJECT_NAME})

set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendors/)
include_directories(
	"${VENDOR_DIR}/re2/"
	"${VENDOR_DIR}/nlohmann_json_cmake_fetchcontent/include/"
	"${CMAKE_SOURCE_DIR}/mdxx/includes/"
)
file(GLOB VENDOR_LIBRARY_DIRS "${VENDOR_DIR}/libs/*")

set(OUT_DIR "${CMAKE_BINARY_DIR}/out")
set(PLUGIN_DIR "${OUT_DIR}/plugins")

link_directories("${VENDOR_LIBRARY_DIRS}" "${OUT_DIR}")

add_subdirectory(mdxx)
add_subdirectory(plugins)
add_subdirectory(vendors)
