cmake_minimum_required (VERSION 3.14)

# Locate GTest
find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests tests.cpp)

add_library(mevent SHARED ../src/Mdatevent.cpp)
target_link_libraries(qmrdump mevent)

add_executable(qmrdump ../src/extractmdat.cpp)
target_link_libraries(mevent GTest::GTest GTest::Main)



add_test(all runTests)

