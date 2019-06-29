cmake_minimum_required (VERSION 3.14)

# Locate GTest
find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library
add_executable(testrunner tests.cpp)

#add_library(gtestmevent SHARED ../src/Mdatevent.cpp)
target_link_libraries(testrunner mevent)

target_link_libraries(testrunner PUBLIC
        ${GTEST_BOTH_LIBRARIES}
)

message(${GTEST_BOTH_LIBRARIES})

#add_executable(gtestqmrdump ../src/extractmdat.cpp)
#target_link_libraries(gtestmevent GTest::GTest GTest::Main)

add_test(NAME all COMMAND testrunner)
