cmake_minimum_required(VERSION 3.0.0)
project(md++)

set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendors/)
include_directories(${VENDOR_DIR}/includes/)
file(GLOB VENDOR_LIBRARY_DIRS "${VENDOR_DIR}/libs/*")
link_directories("${VENDOR_LIBRARY_DIRS}")

add_subdirectory(md++)
