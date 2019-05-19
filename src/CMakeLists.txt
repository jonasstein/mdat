cmake_minimum_required (VERSION 3.9)

enable_testing()

# The version number.
set (QMR_VERSION_MAJOR 1)
set (QMR_VERSION_MINOR 0)


# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	"QMRConfig.h.in"
	"QMRConfig.h"
  )



# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
#include_directories("${PROJECT_BINARY_DIR}")
 
include(CheckCXXCompilerFlag)

# CMAKE_CXX_FLAGS="O2"
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11" )

project (qmr)

# add_executable(qmr main.cpp)
add_executable(qmrdump extractmdat.cpp)
add_library(mevent SHARED Mdatevent.cpp)

