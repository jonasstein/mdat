cmake_minimum_required (VERSION 3.14)

# Locate GTest
find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library
add_executable(Mdatevent_test Mdatevent_test.cpp)

#add_library(gtestmevent SHARED ../src/Mdatevent.cpp)
target_link_libraries(Mdatevent_test Mdatevent ${GTEST_BOTH_LIBRARIES})


add_executable(Bitslicer_test Bitslicer_test.cpp)
target_link_libraries(Bitslicer_test Bitslicer ${GTEST_BOTH_LIBRARIES})

add_executable(Lmfile_test Lmfile_test.cpp)
target_link_libraries(Lmfile_test Lmfile ${GTEST_BOTH_LIBRARIES})

message(STATUS, "WARNING")
message(STATUS, ${GTEST_BOTH_LIBRARIES})

#target_link_libraries(testrunner PUBLIC ${GTEST_BOTH_LIBRARIES})


#add_executable(gtestqmrdump ../src/extractmdat.cpp)
#target_link_libraries(gtestmevent GTest::GTest GTest::Main)

add_test(NAME all COMMAND Mdatevent_test) 
add_test(NAME Bitsclicer COMMAND Bitslicer_test)
add_test(NAME Lmfile COMMAND Lmfile_test)
