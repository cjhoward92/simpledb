add_library(libdb SHARED trie.h trie.c commands.h commands.c)
set_target_properties(libdb PROPERTIES VERSION 1)
set_target_properties(libdb PROPERTIES SOVERSION 1)