add_library(equation-solver STATIC
  quadratic-equation-solver.cpp)

target_include_directories(
  equation-solver PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR})
