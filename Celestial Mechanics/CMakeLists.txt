cmake_minimum_required(VERSION 3.28.1)
project(celestial_mechanics)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(third_party)

add_executable(run_J2 orbit_J2.cpp)
target_link_libraries(run_J2 celestial_mechanics gtest gtest_main sofa Eigen)

enable_testing()
add_subdirectory(tests)

add_executable(oscillator oscillator.cpp)
target_link_libraries(oscillator celestial_mechanics)

add_executable(orbit_Satellite orbit_Satellite.cpp)
target_link_libraries(orbit_Satellite celestial_mechanics)

add_executable(orbit_noSRP orbit_noSRP.cpp)
target_link_libraries(orbit_noSRP celestial_mechanics)

add_executable(C20_firstOrder C20_firstOrder.cpp)
target_link_libraries(C20_firstOrder celestial_mechanics)