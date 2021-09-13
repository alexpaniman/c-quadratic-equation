add_executable(equation-solver-tester
  quadratic-equation-tests.cpp)

target_include_directories(
  equation-solver-tester PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  equation-solver-tester PUBLIC
  equation-solver)

add_test(equation-solver-test equation-solver-tester)
