cmake_minimum_required(VERSION 3.17)
project(Ephemerides)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(third_party)
include_directories(third_party)

set(EXECUTABLE main)
set(TARGET_LIBRS calceph)

add_executable(${EXECUTABLE}_1 example1.cpp)
target_link_libraries(${EXECUTABLE}_1 PUBLIC ${TARGET_LIBRS})

add_executable(${EXECUTABLE}_2 example2.cpp)
target_link_libraries(${EXECUTABLE}_2 PUBLIC ${TARGET_LIBRS})

add_executable(${EXECUTABLE}_3 shadow.cpp)
target_link_libraries(${EXECUTABLE}_3 PUBLIC ${TARGET_LIBRS} Eigen)