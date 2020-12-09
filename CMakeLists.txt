# CMakeLists.txt for libyui
#
# Usage:
#
#   mkdir build
#   cd build
#   cmake ..
#
#   make
#   sudo make install
#
# Restart with a clean build environment:
#   rm -rf build
#
# Show the complete compiler commands with all arguments:
#   make VERBOSE=1

cmake_minimum_required( VERSION 3.10 )
project( libyui )

# Options usage:
#
#   cmake -DBUILD_DOC=on -DBUILD_EXAMPLES=off ..

option( BUILD_EXAMPLES    "Build C++ -based libyui examples"          on  )
option( BUILD_DOC         "Build class documentation"                 off )
option( LEGACY_BUILDTOOLS "Install legacy cmake buildtools"           on  )
option( WERROR            "Treat all compiler warnings as errors"     on  )

# Non-boolean options
set( DOC_DESTDIR "" CACHE STRING "Destination directory prefix for installing docs" )

#----------------------------------------------------------------------


# As of now, no support for /usr/local instead of /usr. But CMake DESTDIR works.
set( CMAKE_INSTALL_PREFIX /usr )


# Initialize compiler flags for all targets in all subdirectories
add_compile_options( "-Wall" )
add_compile_options( "-Os" )    # Optimize for size (overrides CMake's -O3 in RELEASE builds)

if ( WERROR )
  add_compile_options( "-Werror" )
endif()


#
# Descend into subdirectories
#

if ( LEGACY_BUILDTOOLS )
  # Install this first so it doesn't scroll away any important messages
  add_subdirectory( legacy-buildtools )
endif()

add_subdirectory( src )

if ( BUILD_EXAMPLES )
  add_subdirectory( examples )
endif()

if ( BUILD_DOC )
  add_subdirectory( doc )
endif()
