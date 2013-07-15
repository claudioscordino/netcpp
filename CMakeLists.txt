# Version of cmake and programming language:
cmake_minimum_required(VERSION 2.8)

#set (CMAKE_CXX_COMPILER clang++)

# Name of the project:
project(libnetpp)

set (libnetpp_VERSION_MAJOR 0)
set (libnetpp_VERSION_MINOR 1)

set (CMAKE_BUILD_TYPE Release)

# Compiler flags:
add_definitions(-Wextra -O3 -pthread)
set(CMAKE_CXX_FLAGS "-std=c++11")

# Src directories:
subdirs(src test)

# (Absolute) installation directory:
set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/doxygen.conf ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

