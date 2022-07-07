find_package(Check REQUIRED)
include(CheckCSourceCompiles)
include(CheckCSourceRuns)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)

add_executable(tests commands_test.c)
target_link_libraries(tests libdb ${CHECK_LIBRARIES} pthread)

# Create testing target and redirect its output to `Testing` folder
add_test(NAME tests COMMAND tests WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(tests PROPERTIES TIMEOUT 30)

# Additional Valgrind test to check memory leaks in code
include(FindValgrind)

add_custom_target(valgrind_tests
        COMMAND ${VALGRIND_PROGRAM} --leak-check=full $<TARGET_FILE:tests>
        DEPENDS tests
        )

# This is the second signature of add_custom_command, which adds a custom command to a target such as a library or executable. This is useful for performing an operation before or after building the target. The command becomes part of the target and will only execute when the target itself is built. If the target is already built, the command will not execute
add_custom_command(TARGET valgrind_tests
        # On Visual Studio Generators, run before any other rules are executed within the target. On other generators, run just before PRE_LINK commands
        PRE_BUILD
        COMMAND echo -e "executing a PRE_BUILD command"
        COMMENT "This command will be executed before building bar"
        VERBATIM # to support \t for example
        )