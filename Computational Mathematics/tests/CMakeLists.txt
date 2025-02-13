file(GLOB_RECURSE TEST_FILES test_*.cpp)

foreach (FILE IN LISTS TEST_FILES)
    get_filename_component(EXE_NAME ${FILE} NAME_WE)
    set(EXECUTABLE_NAME run_${EXE_NAME})

    add_executable(${EXECUTABLE_NAME} ${FILE})

    target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${PROJECT_NAME} gtest gtest_main)
    add_test(NAME ${EXECUTABLE_NAME} COMMAND ${EXECUTABLE_NAME})
endforeach ()