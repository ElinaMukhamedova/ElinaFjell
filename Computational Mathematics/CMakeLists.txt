cmake_minimum_required(VERSION 3.28.1)
project(numerical_analysis)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(third_party)

enable_testing()
add_subdirectory(tests)

add_subdirectory(advection_equation_main)