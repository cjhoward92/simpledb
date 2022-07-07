cmake_minimum_required(VERSION 3.21)
project(server C)

# Custom cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_C_STANDARD 23)

# Show all warning messages
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-dev")

configure_file(cert.pem cert.pem COPYONLY)
configure_file(key.pem key.pem COPYONLY)

add_subdirectory(lib)
add_subdirectory(src)

enable_testing()

add_subdirectory(tests)