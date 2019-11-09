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



add_library(Histolong SHARED histolong.cpp)
add_executable(mkhistogram mkhistogram.cpp)
target_link_libraries(mkhistogram Histolong)


add_library(Bitslicer SHARED Bitslicer.cpp)
add_library(Mdatevent SHARED Mdatevent.cpp)
target_link_libraries(Mdatevent Bitslicer)
add_library(Lmfile SHARED Lmfile.cpp)
add_executable(qmrdump extractmdat.cpp)
target_link_libraries(qmrdump "stdc++fs" Mdatevent Lmfile Bitslicer)
