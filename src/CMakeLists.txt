include_directories(${CMAKE_SOURCE_DIR}/include)

# Files to be compiled:
add_library(${CMAKE_PROJECT_NAME}_static STATIC logger.cpp posix_socket.cpp  tcp_ip4.cpp)
add_library(${CMAKE_PROJECT_NAME} SHARED logger.cpp posix_socket.cpp  tcp_ip4.cpp)

# Install file myproject. 
# The destination directory is relative to CMAKE_INSTALL_PREFIX:
install (TARGETS ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_static DESTINATION .)
