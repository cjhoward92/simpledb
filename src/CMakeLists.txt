find_package(OpenSSL REQUIRED)

add_executable(server main.c)
target_link_libraries(server OpenSSL::SSL libdb)