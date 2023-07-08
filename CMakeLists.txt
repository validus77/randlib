cmake_minimum_required(VERSION 3.25)
project(randlib)

set(CMAKE_CXX_STANDARD 23)

add_executable(test_randlib tests/tests.cpp include/SkipMap.h)
target_include_directories (test_randlib PUBLIC include)