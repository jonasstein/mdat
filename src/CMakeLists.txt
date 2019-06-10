cmake_minimum_required (VERSION 3.14)
project (qmrdump)

# The version number.
set(QMR_VERSION_MAJOR 0)
set(QMR_VERSION_MINOR 1)
set(QMR_VERSION_PATCH 0)
set(QMR_VERSION
        ${QMR_VERSION_MAJOR}.${QMR_VERSION_MINOR}.${QMR_VERSION_PATCH})


# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	"QMRConfig.h.in"
	"QMRConfig.h"
  )

add_library(mevent SHARED Mdatevent.cpp)
add_executable(qmrdump extractmdat.cpp)
target_link_libraries(qmrdump mevent)
