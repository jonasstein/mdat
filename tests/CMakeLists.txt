cmake_minimum_required (VERSION 3.14)

enable_testing()


# Locate GTest
find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests tests.cpp)

add_test(all runTests)

