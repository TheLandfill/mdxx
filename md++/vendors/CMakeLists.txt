cmake_minimum_required(VERSION 3.0.0)
option(BUILD_SHARED_LIBS "build shared libraries" ON)
add_subdirectory(re2 EXCLUDE_FROM_ALL)
install(TARGETS re2 DESTINATION "${CMAKE_BINARY_DIR}/out")
