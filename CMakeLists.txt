cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(equation-solver VERSION 1.0)

add_subdirectory(lib)

add_executable(equation-solver-front main.cpp)

target_link_libraries(
  equation-solver-front PUBLIC
  equation-solver)
