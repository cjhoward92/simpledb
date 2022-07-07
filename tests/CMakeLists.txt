find_package(Check REQUIRED)
include(CheckCSourceCompiles)
include(CheckCSourceRuns)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)

add_executable(libdb_tests commands_test.c)
target_link_libraries(libdb_tests libdb ${CHECK_LIBRARIES} pthread)

# Create testing target and redirect its output to `Testing` folder
add_test(NAME libdb_tests COMMAND libdb_tests WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(libdb_tests PROPERTIES TIMEOUT 30)

# Additional Valgrind test to check memory leaks in code
include(FindValgrind)

add_custom_target(valgrind_tests
        COMMAND ${VALGRIND_PROGRAM} --leak-check=full $<TARGET_FILE:libdb_tests>
        DEPENDS libdb_tests
        )