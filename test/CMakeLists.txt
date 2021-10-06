# Necessary step, to enable ctest
enable_testing()

add_executable(equation-solver-tester quadratic-equation-tests.cpp)

target_include_directories(equation-solver-tester PUBLIC
			   ${CMAKE_CURRENT_SOURCE_DIR})

# Link library that we're testing
target_link_libraries(equation-solver-tester PUBLIC equation-solver)

# Macro to add test and remeber it's targets
# It's not very useful now, but it will be, when we add more tests
macro(add_unit_test target target_test)
    set(UNIT_TEST_TARGETS ${UNIT_TEST_TARGETS} ${target_test} PARENT_SCOPE)
    add_test(target ${CMAKE_CURRENT_BINARY_DIR}/${target_test})
endmacro(add_unit_test)

# Add test
add_unit_test(equation-solver-test equation-solver-tester)

# Add target that depends on tests' tagets added with add_unit_test
add_custom_target(all_tests ALL DEPENDS ${UNIT_TEST_TARGETS})

# Run tests automatically after build
# --verbose ctest flag always outputs full information about tests,
# it can be replaced with --output-on-failure to do so only when
# some test fails
add_custom_command(TARGET all_tests
                   COMMENT "Run all tests"
                   POST_BUILD COMMAND ctest --verbose
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
