cmake_minimum_required (VERSION 3.14)

# Locate GTest
find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests tests.cpp)

#add_library(gtestmevent SHARED ../src/Mdatevent.cpp)
target_link_libraries(runTests mevent)

#add_executable(gtestqmrdump ../src/extractmdat.cpp)
#target_link_libraries(gtestmevent GTest::GTest GTest::Main)

# add_test(all runTests) # old syntax
add_test(NAME all COMMAND runTests)
