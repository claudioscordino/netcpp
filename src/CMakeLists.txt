include_directories(${netwing_SOURCE_DIR}/include)

# Files to be compiled:
## add_executable(netwing logger.cpp  main.cpp  posix_socket.cpp  tcp_ip4.cpp)
add_library(netwing_static STATIC logger.cpp posix_socket.cpp  tcp_ip4.cpp)
add_library(netwing SHARED logger.cpp posix_socket.cpp  tcp_ip4.cpp)

add_executable (test main.cpp)
target_link_libraries(test netwing_static)

# Install file myproject. 
# The destination directory is relative to CMAKE_INSTALL_PREFIX:
install (TARGETS test netwing netwing_static DESTINATION .)
