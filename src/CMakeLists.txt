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



FIND_PACKAGE( Boost 1.71 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )




# add_library(Histolong SHARED histolong.cpp)
add_library(Histogram SHARED Histogram.cpp)
add_executable(mdathistogram mdathistogram.cpp)
add_library(Timestamps SHARED Timestamps.cpp)
target_link_libraries(mdathistogram LINK_PUBLIC Histogram Timestamps ${Boost_LIBRARIES} )


add_library(Bitslicer SHARED Bitslicer.cpp)
add_library(Mdatevent SHARED Mdatevent.cpp)
add_library(Lmfile SHARED Lmfile.cpp)
target_link_libraries(Mdatevent Bitslicer Timestamps)
target_link_libraries(Lmfile Bitslicer Mdatevent Timestamps)
add_executable(mdatdump mdatdump.cpp)
target_link_libraries(mdatdump LINK_PUBLIC ${Boost_LIBRARIES} Mdatevent Lmfile Bitslicer)


add_executable(mdatshift mdatshift.cpp)
target_link_libraries(mdatshift Timestamps)


add_executable(mdatslice mdatslice.cpp)

