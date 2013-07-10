# Version of cmake and programming language:
cmake_minimum_required(VERSION 2.8)

# Name of the project:
project(libnetpp)

set (CMAKE_BUILD_TYPE Release)

# Compiler flags:
add_definitions(-Wextra -O3 -pthread)
set(CMAKE_CXX_FLAGS "-std=c++11")

# Src directories:
subdirs(src test)

# (Absolute) installation directory:
set (CMAKE_INSTALL_PREFIX /tmp/${CMAKE_PROJECT_NAME})

