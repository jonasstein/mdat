cmake_minimum_required (VERSION 3.14)

enable_testing()
project (qmr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)
add_subdirectory(tests)


include(CheckCXXCompilerFlag)
