find_package(OpenSSL REQUIRED)

add_library(libdb SHARED trie.h trie.c commands.h commands.c)
set_target_properties(libdb PROPERTIES VERSION 1)
set_target_properties(libdb PROPERTIES SOVERSION 1)

add_executable(tls_server main.c)
target_link_libraries(tls_server OpenSSL::SSL libdb)