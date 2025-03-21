include(FetchContent)

FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.16.0
)

FetchContent_MakeAvailable(gtest)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(lz78_test
    ../src/lz78.cpp
    lz78_test.cpp
)

target_link_libraries(lz78_test gtest gtest_main)

add_test(
    NAME lz78_tests
    COMMAND lz78_test
)