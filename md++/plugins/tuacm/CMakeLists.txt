cmake_minimum_required(VERSION 3.0.0)

add_library(tuacm SHARED
	tuacm.cpp
)

target_link_libraries(tuacm mdxx_interface)
if (WIN32)
	install(TARGETS tuacm DESTINATION "${PLUGIN_DIR}")
endif (WIN32)
