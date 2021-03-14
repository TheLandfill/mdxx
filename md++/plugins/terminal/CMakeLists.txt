cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_GENERATOR_PLATFORM x64)

add_library(terminal SHARED
	terminal.cpp
)

target_link_libraries(terminal mdxx_interface)
if (WIN32)
	install(TARGETS terminal DESTINATION "${PLUGIN_DIR}")
endif (WIN32)