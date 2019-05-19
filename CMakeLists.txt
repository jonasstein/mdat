cmake_minimum_required (VERSION 3.9)

# Specify search path for CMake modules to be loaded by include()
# and find_package()
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

enable_testing()
project (qmr)

# disallow in-source build
#include(MacroEnsureOutOfSourceBuild)
#macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build. Please create a separate build directory and run 'cmake /path/to/${PROJECT_NAME} [options]' there.")


# The version number.
#set (QMR_VERSION_MAJOR 1)
#set (QMR_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
#configure_file (
#	"${PROJECT_SOURCE_DIR}/QMRConfig.h.in"
#	"${PROJECT_BINARY_DIR}/QMRConfig.h"
#  )


add_subdirectory(src)
add_subdirectory(tests)


# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
#include_directories("${PROJECT_BINARY_DIR}")
 


#include(CheckCXXCompilerFlag)

# CMAKE_CXX_FLAGS="O2"
#SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11" )


# add_executable(qmr main.cpp)
#add_executable(qmrdump hello.cpp)

#target_link_libraries(qmrdump lm )

# set(CMAKE_BUILD_TYPE Release)

#add_library(lm SHARED lmfile.cpp histogram.cpp)

# Locate GTest
#find_package(GTest REQUIRED)

#include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
#add_executable(runTests tests.cpp)

#add_test(all runTests)


#set(Boost_USE_STATIC_LIBS OFF) 
#set(Boost_USE_MULTITHREADED OFF)  
#set(Boost_USE_STATIC_RUNTIME ON) 
#find_package(Boost 1.56.0 COMPONENTS filesystem program_options) 

#if(Boost_FOUND)
	#  include_directories(${Boost_INCLUDE_DIRS})
	#  target_link_libraries(runTests lm ${GTEST_LIBRARIES} pthread)
	#endif()

