cmake_minimum_required(VERSION 3.21)
project(tls_server C)

set(CMAKE_C_STANDARD 23)

configure_file(cert.pem cert.pem COPYONLY)
configure_file(key.pem key.pem COPYONLY)

find_package(OpenSSL REQUIRED)

add_executable(tls_server main.c trie.h trie.c)
target_link_libraries(tls_server OpenSSL::SSL)
