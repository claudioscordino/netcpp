include_directories(${CMAKE_SOURCE_DIR}/include)

# Files to be compiled:
add_library(${CMAKE_PROJECT_NAME}_static STATIC abstract_socket.cpp logger.cpp posix_socket.cpp tcp_ip4.cpp udp_ip4.cpp stream_local.cpp dgram_local.cpp)
add_library(${CMAKE_PROJECT_NAME} SHARED abstract_socket.cpp logger.cpp posix_socket.cpp tcp_ip4.cpp udp_ip4.cpp stream_local.cpp dgram_local.cpp)

# Install file myproject. 
# The destination directory is relative to CMAKE_INSTALL_PREFIX:
install (TARGETS ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_static DESTINATION .)
