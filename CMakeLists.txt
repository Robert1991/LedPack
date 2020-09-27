cmake_minimum_required(VERSION 3.10)
project(LedHeart CXX)

option(BUILD_TEST "Build the unit tests" ON)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(src)

if(BUILD_TEST)

include(cmake/googletest.cmake)
fetch_googletest(
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_BINARY_DIR}/googletest
    )
 enable_testing()
 add_subdirectory(test)
endif()

