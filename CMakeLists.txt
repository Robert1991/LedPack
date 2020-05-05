cmake_minimum_required(VERSION 3.10)
project(testProject2 CXX)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src)

include(cmake/googletest.cmake)
fetch_googletest(
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_BINARY_DIR}/googletest
    )

enable_testing()


set(SOURCES src/main.cpp src/formula.cpp)
set(TESTS test/main.cpp test/Formula-test.cpp)

enable_testing()
add_subdirectory(test)
