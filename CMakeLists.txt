project(DOMEngine)
cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

add_definitions(-DAPP_VERSION="1.1")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

include_directories(
	include
)

set(SRC
	src/main.cpp
	src/node.cpp
	src/tree.cpp
	src/parser.cpp
	src/util.cpp
	src/vm.cpp
	src/script.cpp
	src/shell.cpp
)

add_executable(dom-engine ${SRC})

if (UNIX)
	install(TARGETS dom-engine RUNTIME DESTINATION /usr/bin/)
else()
	message(STATUS "DOMEngine only installs on Linux at the moment.")
endif()