file(GLOB_RECURSE TEST_FILES test_*.cpp)

foreach (FILE IN LISTS TEST_FILES)
    get_filename_component(EXE_NAME ${FILE} NAME_WE)
    set(EXECUTABLE_NAME run_${EXE_NAME})

    add_executable(${EXECUTABLE_NAME} ${FILE})

    target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${PROJECT_NAME} gtest gtest_main)
    target_include_directories(${EXECUTABLE_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
    add_test(NAME ${EXECUTABLE_NAME} COMMAND ${EXECUTABLE_NAME})
endforeach ()