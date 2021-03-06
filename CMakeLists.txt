# Version of cmake and programming language:
cmake_minimum_required(VERSION 2.8)

#set (CMAKE_CXX_COMPILER clang++)

# Name of the project:
project(netcpp)

set (netcpp_VERSION_MAJOR 0)
set (netcpp_VERSION_MINOR 2)

set (CMAKE_BUILD_TYPE Debug)
## set (CMAKE_BUILD_TYPE Release)

# Compiler flags:
add_definitions(-Wall -Wextra -Werror -O3 -pthread -DLOG_LEVEL=3)
set(CMAKE_CXX_FLAGS "-std=c++11")

# Src directories:
subdirs(src test)

# (Absolute) installation directory:
set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/doxygen.conf ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(do_doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
add_custom_target(doc
COMMAND ln -sf ${CMAKE_INSTALL_PREFIX}/doc/html/index.html  ${CMAKE_INSTALL_PREFIX}/index.html
DEPENDS do_doc cp_logo
)
add_custom_target(cp_logo
COMMAND cp -f ${CMAKE_CURRENT_SOURCE_DIR}/doc/*.png ${CMAKE_INSTALL_PREFIX}/doc/html/
DEPENDS do_doc
)
endif(DOXYGEN_FOUND)

