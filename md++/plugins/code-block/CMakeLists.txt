cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_GENERATOR_PLATFORM x64)

find_package(PythonLibs REQUIRED)
add_library(py STATIC IMPORTED)
set_property(TARGET py PROPERTY IMPORTED_LOCATION ${PYTHON_LIBRARIES})

add_library(code-block SHARED
	code-block.cpp
)

include_directories(${PYTHON_INCLUDE_DIRS})

target_link_libraries(code-block mdxx_interface py)
if (WIN32)
	install(TARGETS code-block DESTINATION "${PLUGIN_DIR}")
endif (WIN32)
install(FILES code_block.py DESTINATION "${PLUGIN_DIR}")
