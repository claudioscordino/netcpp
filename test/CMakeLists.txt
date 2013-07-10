include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable (test main.cpp)
target_link_libraries(test ${CMAKE_PROJECT_NAME}_static pthread)

# Install file myproject. 
# The destination directory is relative to CMAKE_INSTALL_PREFIX:
install (TARGETS test DESTINATION .)
