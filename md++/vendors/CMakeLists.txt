cmake_minimum_required(VERSION 3.0.0)
add_subdirectory(re2 EXCLUDE_FROM_ALL)
set_property(TARGET re2 PROPERTY POSITION_INDEPENDENT_CODE ON)
