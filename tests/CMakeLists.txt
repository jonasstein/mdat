cmake_minimum_required (VERSION 3.14)

# Locate GTest
find_package(GTest REQUIRED)

add_executable(Mdatevent_test Mdatevent_test.cpp)
target_link_libraries(Mdatevent_test Mdatevent ${GTEST_BOTH_LIBRARIES})

add_executable(Histogram_test Histogram_test.cpp)
target_link_libraries(Histogram_test Histogram ${GTEST_BOTH_LIBRARIES})

add_executable(Bitslicer_test Bitslicer_test.cpp)
target_link_libraries(Bitslicer_test Bitslicer ${GTEST_BOTH_LIBRARIES})

add_executable(Lmfile_test Lmfile_test.cpp)
target_link_libraries(Lmfile_test Lmfile ${GTEST_BOTH_LIBRARIES})

message(STATUS, "WARNING")
message(STATUS, ${GTEST_BOTH_LIBRARIES})

add_test(NAME all COMMAND Mdatevent_test) 
add_test(NAME Bitsclicer COMMAND Bitslicer_test)
add_test(NAME Histogram COMMAND Histogram_test)
add_test(NAME Lmfile COMMAND Lmfile_test)
