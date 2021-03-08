cmake_minimum_required(VERSION 3.0.0)

find_package(PythonLibs REQUIRED)

add_library(code-block SHARED
	code-block.cpp
)

include_directories(${PYTHON_INCLUDE_DIRS})

target_link_libraries(code-block mdxx_interface ${PYTHON_LIBRARIES})
if (WIN32)
	install(TARGETS code-block DESTINATION "${PLUGIN_DIR}")
endif (WIN32)
install(FILES code_block.py DESTINATION "${PLUGIN_DIR}")
